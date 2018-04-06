#ifndef _ENGINE_GRAPHICS_SHADER_H_
#define _ENGINE_GRAPHICS_SHADER_H_

#include "core\Types.h"
#include "glm\glm.hpp"
#include "core\Object.h"

#include "ShaderUniform.h"

#include "ShaderType.h"

namespace engine
{
  namespace graphics
  {
    class Material;

    class Shader : public core::Object, public NonCopyable
    {
      friend class Mesh;
      friend class Material;
      friend class DefaultRenderer;

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

      void setUniform(GLint _location, GLenum _type, const void * _data);

      bool AddUniform(const std::string & _name, GLenum _type, uint _size, ShaderUniform * _outUniform);
      template <typename T>
      bool AddUniform(const std::string & _name, ShaderUniform * _outUniform);
      bool getUniform(const std::string & _name, ShaderUniform * _outUniform) const;

      bool AddTexture(const std::string & _name, ShaderTexture * _outTexture);
      bool getTexture(const std::string & _name, ShaderTexture * _outTexture) const;

      bool AddAttribute(const std::string & _name, GLenum _type, uint _count, bool _normalize, ShaderAttribute * _outAttribute);
      template <typename T>
      bool AddAttribute(const std::string & _name, bool _normalized, ShaderAttribute * _outAttribute);
      bool getAttribute(const std::string & _name, ShaderAttribute * _outAttribute) const;

      void ResizeUniformBuffer(size_t _size);
      void UpdateMaterials();

      void AddMaterial(Material * _material);
      void RemoveMaterial(Material * _material);

      GLuint m_program;
      std::vector<GLuint> m_shaders;

      std::vector<ShaderAttribute> m_attributes;
      std::map<std::string, size_t> m_nameToAttribute;

      std::vector<ShaderUniform> m_uniforms;
      std::map<std::string, size_t> m_nameToUniform;
      uint m_uniformSize;

      std::vector<ShaderTexture> m_textures;
      std::map<std::string, size_t> m_nameToTexture;

      GLint m_modelLoc, m_viewLoc, m_projectionLoc;

      std::set<Material *> m_materials;
    };
  }
}

#include "Shader.inl"

#endif //_ENGINE_GRAPHICS_SHADER_H_