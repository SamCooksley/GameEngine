#ifndef _ENGINE_UTILITIES_WAVEFRONTPARSER_H_
#define _ENGINE_UTILITIES_WAVEFRONTPARSER_H_

#include "graphics\MeshData.h"

namespace engine {
namespace file {

  class WavefrontParser
  {
   public:
    WavefrontParser(const String & _path);
    WavefrontParser(const String & _name, std::istream & _source);
    ~WavefrontParser();
  
    graphics::MeshData getMesh() const;
  
    const String & getName() const;
  
   private:
    void Parse(std::istream & _source);
  
    uint32 getMaxIndex() const;
  
   private:
    String m_name;
  
    String m_file;
  
    uint32 m_maxIndex;
  
    std::vector<uint32> m_indices;
    std::vector<glm::vec3> m_vertices;
    std::vector<glm::vec2> m_uvs;
    std::vector<glm::vec3> m_normals;
  };

} } // engine::file

#endif //_ENGINE_UTILITIES_WAVEFRONTPARSER_H_