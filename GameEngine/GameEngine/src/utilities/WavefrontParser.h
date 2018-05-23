#ifndef _ENGINE_UTILITIES_WAVEFRONTPARSER_H_
#define _ENGINE_UTILITIES_WAVEFRONTPARSER_H_

#include "core\Types.h"
#include "graphics\MeshData.h"

namespace engine
{
  namespace file
  {
    class WavefrontParser
    {
    public:
      WavefrontParser(const std::string & _path);
      WavefrontParser(const std::string & _name, std::istream & _source);
      ~WavefrontParser();

      graphics::MeshData getMesh() const;

      const std::string & getName() const;

    private:
      void Parse(std::istream & _source);

      uint32 getMaxIndex() const;

      std::string m_name;

      std::string m_file;

      uint32 m_maxIndex;

      std::vector<uint32> m_indices;
      std::vector<glm::vec3> m_vertices;
      std::vector<glm::vec2> m_uvs;
      std::vector<glm::vec3> m_normals;
    };
  }
}

#endif //_ENGINE_UTILITIES_WAVEFRONTPARSER_H_