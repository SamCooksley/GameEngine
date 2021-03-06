#include "stdafx.h"

#include <sstream>

#include "WavefrontParser.h"

#include "File.h"

namespace engine {
namespace file {

  WavefrontParser::WavefrontParser(const String & _path) :
    m_name(), m_file(), m_maxIndex(0)
  {
    m_name = file::getFilenameWithoutExtension(_path);
  
    if (file::getFileExtension(_path) != ".obj")
    {
      throw std::runtime_error(
        "WavefrontParser Error: " + m_name + " invalid file type: " + _path
      );
    }
  
    std::ifstream file(_path);
    if (!file.is_open())
    {
      throw std::runtime_error(
        "WavefrontParser Error: " + m_name + " failed to open file: " + _path
      );
    }
    m_file = _path;
  
    Parse(file);
  }
  
  WavefrontParser::WavefrontParser(const String & _name, std::istream & _source) :
    m_name(_name), m_file(), m_maxIndex(0)
  {
    Parse(_source);
  }
  
  WavefrontParser::~WavefrontParser()
  { }
  
  graphics::MeshData WavefrontParser::getMesh() const
  {
    graphics::MeshData mesh;
  
    mesh.indices.setIndices(m_indices);
  
    mesh.positions = m_vertices;
    mesh.uvs = m_uvs;
    mesh.normals = m_normals;
    
    return std::move(mesh);
  }
  
  const String & WavefrontParser::getName() const
  {
    return m_name;
  }
  
  void WavefrontParser::Parse(std::istream & _source)
  {
    struct VertexIndex
    {
      uint position;
      uint uv;
      uint normal;
  
      bool operator<(const VertexIndex & _other) const
      {
        return memcmp(this, &_other, sizeof(VertexIndex)) < 0;
      }
    };
  
    std::vector<glm::vec3> vertexData;
    std::vector<glm::vec2> uvData;
    std::vector<glm::vec3> normalData;
  
    std::map<VertexIndex, uint32> used;
  
    String line;
  
    while (std::getline(_source, line))
    {
      std::stringstream lineStream(line);
  
      if (line.substr(0, 2) == "vt")
      {
        String tmp;
        float x, y;
        lineStream >> tmp >> x >> y;
        uvData.emplace_back(x, y);
      }
      else if (line.substr(0, 2) == "vn")
      {
        String tmp;
        float x, y, z;
        lineStream >> tmp >> x >> y >> z;
        normalData.emplace_back(x, y, z);
      }
      else if (line[0] == 'v')
      {
        String tmp;
        float x, y, z;
        lineStream >> tmp >> x >> y >> z;
        vertexData.emplace_back(x, y, z);
      }
      else if (line[0] == 'f')
      {
        char tmp;
        String vertex[4];
  
        lineStream >> tmp >> vertex[0] >> vertex[1] >> vertex[2] >> vertex[3];
  
        if (!vertex[3].empty())
        {
          throw std::range_error(
            "WavefrontParser Error: " + m_name + " quad mesh detected. Not supported"
          );
        }
  
        for (size_t i = 0u; i < 3u; ++i)
        {
          const String & vert = vertex[i];
          std::stringstream vertStream(vert);
  
          VertexIndex index = { 0, 0, 0 };
  
          if (vert.find('/') == String::npos)
          {
            vertStream >> index.position;
          }
          else if (vert.find("//") != String::npos)
          {
            char tmp;
            vertStream >> index.position >> tmp >> tmp >> index.normal;
          }
          else
          {
            char tmp;
            vertStream >> index.position >> tmp >> index.uv >> tmp >> index.normal;
          }
  
          if (index.position == 0)
          {
            throw std::range_error(
              "WavefrontParser Error: " + m_name + " missing vertex position"
            );
          }
  
          auto iter = used.find(index);
  
          if (iter == used.end())
          {
            m_indices.push_back(m_vertices.size());
            used[index] = m_indices.back();
  
            glm::vec3 v, n; glm::vec2 u;
  
            v = vertexData[index.position - 1];
            m_vertices.push_back(v);
            if (index.uv > 0)  
            {
              u = uvData[index.uv - 1];  
              m_uvs.push_back(u);
            }
            if (index.normal > 0) 
            {
              n = normalData[index.normal - 1];
              m_normals.push_back(n);
            }
          }
          else
          {
            m_indices.push_back(iter->second);
          }
        }
      }
    }
  
    m_maxIndex = getMaxIndex();
  }
  
  uint32 WavefrontParser::getMaxIndex() const
  {
    uint32 max = 0;
    for (const uint32 & index : m_indices)
    {
      max = std::max(max, index);
    }
    return max;
  }
 
} } // engine::file