#ifndef _ENGINE_GRAPHICS_SHADER_H_
#define _ENGINE_GRAPHICS_SHADER_H_

#include "core\Types.h"
#include "glm\glm.hpp"
#include "core\Object.h"
#include "ShaderParser.h"

namespace engine { namespace graphics {

  class Shader : public core::Object
  {
  public:
    static void Load(const std::string & _path);

    ~Shader();

    void Bind();
    void Unbind();

    void setUniform(const std::string& _name, int _val);
    void setUniform(const std::string& _name, float _val); 
    void setUniform(const std::string& _name, const glm::vec2& _vec); 
    void setUniform(const std::string& _name, const glm::vec3& _vec); 
    void setUniform(const std::string& _name, const glm::vec4& _vec); 
    void setUniform(const std::string& _name, const glm::mat3& _mat); 
    void setUniform(const std::string& _name, const glm::mat4& _mat); 

  protected:
    Shader();

  private:
    bool AddShader(const std::string & _source, ShaderType _type);
    bool LinkProgram();
    void DetachAndDeleteShaders();

    uint m_program;
    std::vector<uint> m_shaders;
  };

} }

#endif //_ENGINE_GRAPHICS_SHADER_H_