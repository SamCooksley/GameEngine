#ifndef _ENGINE_GRAPHICS_SHADER_H_
#define _ENGINE_GRAPHICS_SHADER_H_

#include "core\Types.h"
#include "glm\glm.hpp"

#include "Asset.h"

#include "core\NamedObject.h"

#include "ShaderUniform.h"

#include "ShaderType.h"

#include "UniformBuffer.h"

#include "GLData.h"

namespace engine
{
  namespace graphics
  {
    class Material;

    class Shader : public NamedObject, public NonCopyable, public Asset
    {
      friend class Mesh;
      friend class Material;
      friend class DefaultRenderer;
      friend class UniformBuffers;

    public:
      static const std::string PROJECTION_NAME;
      static const std::string VIEW_NAME;
      static const std::string MODEL_NAME;

      static const std::string ATTR_POSITION_NAME;
      static const std::string ATTR_UV_NAME;
      static const std::string ATTR_NORMAL_NAME;
      static const std::string ATTR_TANGENT_NAME;
      static const std::string ATTR_BITANGENT_NAME;

      static std::shared_ptr<Shader> Load(const std::string & _path);

      ~Shader();

      void Bind() const;
      void Unbind() const;

      void setModel(const glm::mat4 & _model);
      void setView(const glm::mat4 & _view);
      void setProjection(const glm::mat4 & _projection);

    protected:
      Shader();

    private:
      void CreateShader(const std::string & _source, ShaderType::Type _type);
      void LinkProgram();
      void DetachAndDeleteShaders();

      void SetupSystemUniforms();
      void SetupAttributes();
      void SetupUniformBuffers();
      void SetupUniforms();

      void SetupUniformBuffer(const UniformBuffer & _ub);

      bool SetSystemUniform(const std::string & _name, GLint _location);

      void RetreiveUniformData(std::vector<byte> & _outData);

      template <typename T>
      void RetreiveUniformValue(const ShaderUniform & _uniform, std::vector<byte> & _dest) const;

      template <typename T>
      T RetreiveUniformValue(const ShaderUniform & _uniform) const;

      GLint getUniformLocation(const std::string & _name) const;
      GLint getAttributeLocation(const std::string & _name) const;

      GLuint getUniformBlockIndex(const std::string & _name) const;

      void setUniformBlockBinding(GLuint _index, GLuint _bind);

      void setUniform(GLint _location, int _val);
      void setUniform(GLint _location, float _val);
      void setUniform(GLint _location, const glm::vec2& _vec);
      void setUniform(GLint _location, const glm::vec3& _vec);
      void setUniform(GLint _location, const glm::vec4& _vec);
      void setUniform(GLint _location, const glm::mat3& _mat);
      void setUniform(GLint _location, const glm::mat4& _mat);

      void setUniform(GLint _location, Type _type, const void * _data);

      bool getAttribute(const std::string & _name, ShaderAttribute * _outAttribute) const;
      bool getUniform(const std::string & _name, ShaderUniform * _outUniform) const;
      bool getSampler(const std::string & _name, ShaderSampler * _outSampler) const;

      static void AddShader(const std::shared_ptr<Shader> & _shader);
      void RemoveShader();

      GLuint m_program;
      std::vector<GLuint> m_shaders;

      Dictionary<std::string, ShaderAttribute> m_attributes;

      Dictionary<std::string, ShaderUniform> m_uniforms;
      uint m_uniformSize;
      Dictionary<std::string, ShaderSampler> m_samplers;

      GLint m_modelLoc, m_viewLoc, m_projectionLoc;

      Depth::Func m_depth;
      Cull::Face m_cull;
      Blend m_blend;
    };
  }
}

#include "Shader.inl"

#endif //_ENGINE_GRAPHICS_SHADER_H_