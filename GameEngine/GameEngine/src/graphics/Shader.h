#ifndef _ENGINE_GRAPHICS_SHADER_H_
#define _ENGINE_GRAPHICS_SHADER_H_

#include "core\Types.h"
#include "Asset.h"

#include "ShaderType.h"

#include "ShaderUniform.h"
#include "UniformBuffer.h"

#include "GLData.h"
#include "RenderQueue.h"

namespace engine {
namespace graphics {
  class Material;
  
  class Shader : public NamedObject, public NonCopyable, public Asset, public std::enable_shared_from_this<Shader>
  {
    friend class Mesh;
    friend class Material;
    friend class DefaultRenderer;
    friend class UniformBuffers;
  
   public:
    static const String PROJECTION_NAME;
    static const String VIEW_NAME;
    static const String MODEL_NAME;
  
    static const String ATTR_POSITION_NAME;
    static const String ATTR_UV_NAME;
    static const String ATTR_NORMAL_NAME;
    static const String ATTR_TANGENT_NAME;
    static const String ATTR_BITANGENT_NAME;
  
    static std::shared_ptr<Shader> Load(const String & _path);
  
   public:
    ~Shader();
  
    void Bind();
  
    void setModel(const glm::mat4 & _model);
    void setView(const glm::mat4 & _view);
    void setProjection(const glm::mat4 & _projection);
  
    template <typename T>
    void setUniform(const String & _name, const T & _value);

    template <typename T>
    T getUniform(const String & _name) const;

    RenderQueue::Queue getRenderQueue() const;
  
   protected:
    Shader();
  
   private:
    void CreateShader(const String & _source, ShaderType::Type _type);
    void LinkProgram();
    void DetachAndDeleteShaders();
  
    void SetupSystemUniforms();
    void SetupAttributes();
    void SetupUniformBuffers();
    void SetupUniforms();
  
    void SetupUniformBuffer(const UniformBuffer & _ub);
  
    bool SetSystemUniform(const String & _name, GLint _location);
  
    void RetreiveUniformData(std::vector<byte> & _outData);
  
    template <typename T>
    void RetreiveUniformValue(const ShaderUniform & _uniform, std::vector<byte> & _dest) const;
  
    template <typename T>
    T RetreiveUniformValue(const ShaderUniform & _uniform) const;
  
    GLint getUniformLocation(const String & _name) const;
    GLint getAttributeLocation(const String & _name) const;
  
    GLuint getUniformBlockIndex(const String & _name) const;
  
    void setUniformBlockBinding(GLuint _index, GLuint _bind);
  
    void setUniform(GLint _location, int _val);
    void setUniform(GLint _location, float _val);
    void setUniform(GLint _location, const glm::vec2& _vec);
    void setUniform(GLint _location, const glm::vec3& _vec);
    void setUniform(GLint _location, const glm::vec4& _vec);
    void setUniform(GLint _location, const glm::mat3& _mat);
    void setUniform(GLint _location, const glm::mat4& _mat);
  
    void setUniform(GLint _location, Type _type, const void * _data);
  
    bool getAttribute(const String & _name, ShaderAttribute * _outAttribute) const;
    bool getUniform(const String & _name, ShaderUniform * _outUniform) const;
    bool getSampler(const String & _name, ShaderSampler * _outSampler) const;
  
    static void AddShader(const std::shared_ptr<Shader> & _shader);
    void RemoveShader();

   private:  
    GLuint m_program;
    std::vector<GLuint> m_shaders;
  
    Dictionary<String, ShaderAttribute> m_attributes;
  
    Dictionary<String, ShaderUniform> m_uniforms;
    uint m_uniformSize;
    Dictionary<String, ShaderSampler> m_samplers;
  
    GLint m_modelLoc, m_viewLoc, m_projectionLoc;
  
    bool m_depthWrite;
    Depth::Func m_depth;
    Cull::Face m_cull;
    Blend m_blend;
    RenderQueue::Queue m_queue;
  };
  
} } // engine::graphics

#include "Shader.inl"

#endif //_ENGINE_GRAPHICS_SHADER_H_