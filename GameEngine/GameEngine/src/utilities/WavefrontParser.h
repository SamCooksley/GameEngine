#ifndef _ENGINE_UTILITIES_WAVEFRONTPARSER_H_
#define _ENGINE_UTILITIES_WAVEFRONTPARSER_H_

#include "core\Types.h"

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

      bool CanUseIndex8() const;
      bool CanUseIndex16() const;

      void getIndices(std::vector<uint8> & _out) const;
      void getIndices(std::vector<uint16> & _out) const;
      void getIndices(std::vector<uint32> & _out) const;

      const std::vector<uint32> & getIndices() const;

      bool HasUVs() const;
      bool HasNormals() const;

      const std::vector<glm::vec3> & getVertices() const;
      const std::vector<glm::vec2> & getUVs() const;
      const std::vector<glm::vec3> & getNormals() const;

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