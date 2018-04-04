#include "stdafx.h"

#include <sstream>

#include "WavefrontParser.h"

#include "File.h"

namespace engine
{
  namespace file
  {
    WavefrontParser::WavefrontParser(const std::string & _path) :
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

    WavefrontParser::WavefrontParser(const std::string & _name, std::istream & _source) :
      m_name(_name), m_file(), m_maxIndex(0)
    {
      Parse(_source);
    }

    WavefrontParser::~WavefrontParser()
    { }

    bool WavefrontParser::CanUseIndex8() const
    {
      return m_maxIndex < std::numeric_limits<uint8>().max();
    }

    bool WavefrontParser::CanUseIndex16() const
    {
      return m_maxIndex < std::numeric_limits<uint16>().max();
    }

    void WavefrontParser::getIndices(std::vector<uint8> & _out) const
    {
      if (!CanUseIndex8())
      {
        throw std::overflow_error(
          "WavefrontParser Error: " + m_name + " indices cannot be converted to uint8"
        );
      }

      _out.resize(m_indices.size());
      for (size_t i = 0; i < m_indices.size(); ++i)
      {
        _out[i] = m_indices[i];
      }
    }

    void WavefrontParser::getIndices(std::vector<uint16> & _out) const
    {
      if (!CanUseIndex16())
      {
        throw std::overflow_error(
          "WavefrontParser Error: " + m_name + " indices cannot be converted to uint16"
        );
      }

      _out.resize(m_indices.size());
      for (size_t i = 0; i < m_indices.size(); ++i)
      {
        _out[i] = m_indices[i];
      }
    }

    void WavefrontParser::getIndices(std::vector<uint32> & _out) const
    {
      _out = m_indices;
    }

    const std::vector<uint32> & WavefrontParser::getIndices() const
    {
      return m_indices;
    }

    bool WavefrontParser::HasUVs() const
    {
      return !m_uvs.empty();
    }

    bool WavefrontParser::HasNormals() const
    {
      return !m_normals.empty();
    }

    const std::vector<glm::vec3> & WavefrontParser::getVertices() const
    {
      return m_vertices;
    }

    const std::vector<glm::vec2> & WavefrontParser::getUVs() const
    {
      return m_uvs;
    }

    const std::vector<glm::vec3> & WavefrontParser::getNormals() const
    {
      return m_normals;
    }

    const std::string & WavefrontParser::getName() const
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

      std::string line;

      while (std::getline(_source, line))
      {
        std::stringstream lineStream(line);

        if (line.substr(0, 2) == "vt")
        {
          std::string tmp;
          float x, y;
          lineStream >> tmp >> x >> y;
          uvData.emplace_back(x, y);
        }
        else if (line.substr(0, 2) == "vn")
        {
          std::string tmp;
          float x, y, z;
          lineStream >> tmp >> x >> y >> z;
          normalData.emplace_back(x, y, z);
        }
        else if (line[0] == 'v')
        {
          std::string tmp;
          float x, y, z;
          lineStream >> tmp >> x >> y >> z;
          vertexData.emplace_back(x, y, z);
        }
        else if (line[0] == 'f')
        {
          char tmp;
          std::string vertex[4];

          lineStream >> tmp >> vertex[0] >> vertex[1] >> vertex[2] >> vertex[3];

          if (!vertex[3].empty())
          {
            throw std::range_error(
              "WavefrontParser Error: " + m_name + " quad mesh detected. Not supported"
            );
          }

          for (size_t i = 0u; i < 3u; ++i)
          {
            const std::string & vert = vertex[i];
            std::stringstream vertStream(vert);

            VertexIndex index = { 0, 0, 0 };

            if (vert.find('/') == std::string::npos)
            {
              vertStream >> index.position;
            }
            else if (vert.find("//") != std::string::npos)
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
  }
}