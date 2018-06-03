#include "stdafx.h"

#include "Shader.h"

#include "Material.h"

#include "utilities\ShaderParser.h"

#include "Graphics.h"

namespace engine {
namespace graphics {

  static bool IsSupportedSamplerUniformType(GLenum _type)
  {
    try
    {
      OpenGLToTextureTypeSampler(_type);
      return true;
    }
    catch (std::invalid_argument&)
    {
      return false;
    }
  }
  
  static bool IsSupportedUniformType(GLenum _type)
  {
    try
    {
      OpenGLToType(_type);
      return true;
    }
    catch (std::invalid_argument&)
    { }
  
    return IsSupportedSamplerUniformType(_type);
  }
  
  const String Shader::PROJECTION_NAME = "projection";
  const String Shader::VIEW_NAME       = "view";
  const String Shader::MODEL_NAME      = "model";
  
  const String Shader::ATTR_POSITION_NAME  = "in_position";
  const String Shader::ATTR_UV_NAME        = "in_texCoords";
  const String Shader::ATTR_NORMAL_NAME    = "in_normal";
  const String Shader::ATTR_TANGENT_NAME   = "in_tangent";
  const String Shader::ATTR_BITANGENT_NAME = "in_bitangent";
  
  std::shared_ptr<Shader> Shader::Load(const String & _path)
  {
    class enable_shader : public Shader { };
  
    std::shared_ptr<Shader> shader;
  
    try
    {
      file::ShaderParser parser(_path);
      shader = std::make_shared<enable_shader>();
      shader->setName(parser.getName());
  
      shader->m_depth = parser.getDepth();
      shader->m_cull = parser.getCull();
      shader->m_blend = parser.getBlend();
      shader->m_queue = parser.getQueue();
  
      for (int i = ShaderType::VERTEX; i <= ShaderType::GEOMETRY; ++i)
      {
        ShaderType::Type type = static_cast<ShaderType::Type>(i);
        if (parser.HasShader(type))
        {
          shader->CreateShader(parser.getShaderSource(type), type);
        }
      }
  
      shader->LinkProgram();
      shader->DetachAndDeleteShaders();
  
      shader->SetupUniformBuffers();
      shader->SetupAttributes();
      shader->SetupUniforms();
  
      Shader::AddShader(shader);
    }
    catch (std::exception & _e)
    {
      debug::LogError(_e.what());
      return Graphics::getErrorShader();
    }
  
    return shader;
  }
  
  Shader::Shader() :
    m_uniformSize(0u),
    m_modelLoc(-1), m_viewLoc(-1), m_projectionLoc(-1),
    m_depth(Depth::LESS), m_cull(Cull::BACK), 
    m_blend(Blend::Disable())
  {
    GLCALL(m_program = glCreateProgram());
  }
  
  Shader::~Shader()
  {
    RemoveShader();
  
    GLCALL(glDeleteProgram(m_program));
  }
  
  void Shader::Bind()
  {
    if (this != Graphics::getContext().activeShader.lock().get())
    {
      GLCALL(glUseProgram(m_program));
  
      Graphics::GL().SetDepth(m_depth);
      Graphics::GL().SetCull(m_cull);
      Graphics::GL().SetBlend(m_blend);
  
      Graphics::getContext().activeShader = shared_from_this();
    }
  }
  
  void Shader::Unbind()
  {
    GLCALL(glUseProgram(0));
    Graphics::getContext().activeShader.reset();
  }
  
  void Shader::CreateShader(const String & _source, ShaderType::Type _type)
  {
    GLCALL(GLuint shader = glCreateShader(ShaderType::ToOpenGL(_type)));
  
    const char* source = _source.c_str();
    GLCALL(glShaderSource(shader, 1, &source, nullptr));
  
    GLCALL(glCompileShader(shader));
  
    GLint compiled;
    GLCALL(glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled));
  
    if (compiled == GL_FALSE)
    {
      GLsizei length;
      GLCALL(glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length));
      std::vector<GLchar> log(length);
      GLCALL(glGetShaderInfoLog(shader, length, &length, &log[0]));
  
      GLCALL(glDeleteShader(shader));
  
      throw std::runtime_error(
        "Shader Error: " + getName() + " compilation failed. " + String(&log[0], length)
      );
    }
  
    GLCALL(glAttachShader(m_program, shader));
    m_shaders.push_back(shader);
  }
  
  void Shader::LinkProgram()
  {
    GLCALL(glLinkProgram(m_program));
  
    GLint linked;
    glGetProgramiv(m_program, GL_LINK_STATUS, &linked);
  
    if (linked == GL_FALSE)
    {
      GLsizei length;
      GLCALL(glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &length));
      std::vector<GLchar> log(length);
      GLCALL(glGetProgramInfoLog(m_program, length, &length, &log[0]));
  
      throw std::runtime_error(
        "Shader Error: " + getName() + " linking failed. " + String(&log[0], length)
      );
    }
  }
  
  void Shader::DetachAndDeleteShaders()
  {
    for (auto shader : m_shaders)
    {
      GLCALL(glDetachShader(m_program, shader));
      GLCALL(glDeleteShader(shader));
    }
    m_shaders.clear();
  }
  
  void Shader::SetupSystemUniforms()
  {
    m_modelLoc = getUniformLocation(MODEL_NAME);
    m_viewLoc = getUniformLocation(VIEW_NAME);
    m_projectionLoc = getUniformLocation(PROJECTION_NAME);
  }
  
  void Shader::SetupUniformBuffers()
  {
    auto & buffers = Graphics::getContext().uniformBuffers.m_buffers;
    for (auto & buffer : buffers)
    {
      SetupUniformBuffer(*buffer.second);
    }
  }
  
  void Shader::SetupUniformBuffer(const UniformBuffer & _ub)
  {
    GLuint index = getUniformBlockIndex(_ub.getName());
    if (index != GL_INVALID_INDEX)
    {
      setUniformBlockBinding(index, _ub.getBind());
    }
  }
  
  void Shader::SetupAttributes()
  {
    GLint scount;
    GLCALL(glGetProgramiv(m_program, GL_ACTIVE_ATTRIBUTES, &scount));
  
    if (scount < 0) { return; }
    GLuint count = scount;
  
    GLint maxLength;
    GLCALL(glGetProgramiv(m_program, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxLength));
  
    std::vector<GLchar> nameBuffer(maxLength);
  
    for (GLuint i = 0; i < count; ++i)
    {
      GLenum type;
      GLsizei length;
      GLint size;
      GLCALL(glGetActiveAttrib(m_program, i, maxLength, &length, &size, &type, &nameBuffer[0]));
  
      String name(nameBuffer.begin(), nameBuffer.begin() + length);
  
      GLint loc = getAttributeLocation(name);
      
      if (loc < 0) { continue; }
      
      ShaderAttribute attribute;
      attribute.name = name;
      attribute.type = OpenGLToType(type);
      attribute.location = loc;
  
      m_attributes.add(name, attribute);
    }
  }
  
  void Shader::SetupUniforms()
  {
    GLint scount;
    GLCALL(glGetProgramiv(m_program, GL_ACTIVE_UNIFORMS, &scount));
  
    if (scount < 0) { return; }
    GLuint count = scount;
  
    GLint maxLength;
    GLCALL(glGetProgramiv(m_program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxLength));
   
    std::vector<GLchar> nameBuffer(maxLength);
  
    for (GLuint i = 0; i < count; ++i)
    {
      GLenum type;
      GLsizei length;
      GLint size;
  
      GLCALL(glGetActiveUniform(m_program, i, maxLength, &length, &size, &type, &nameBuffer[0]));
  
      String name(nameBuffer.begin(), nameBuffer.begin() + length);
  
      if (!IsSupportedUniformType(type))
      {
        debug::LogError("Shader " + getName() + " uniform " + name + " invalid type: " + std::to_string(type));
        continue;
      }
  
      GLint loc = getUniformLocation(name);
      //remove invalid uniforms (uniform buffer objects).
      if (loc < 0) { continue; }  
  
      if (SetSystemUniform(name, loc))
      {
        continue;
      }
  
      bool isSampler = IsSupportedSamplerUniformType(type);
  
      ShaderUniform uniform;
      uniform.name = name;
      uniform.type = isSampler ? Type::INT : OpenGLToType(type);
      uniform.size = GetTypeSize(uniform.type);
      uniform.location = loc;
  
      uniform.offset = m_uniformSize;
      m_uniformSize += uniform.size;
  
      m_uniforms.add(name, uniform);
  
      if (isSampler)
      {
        ShaderSampler sampler;
        sampler.name = name;
        sampler.type = OpenGLToTextureTypeSampler(type);
        //should be okay as elements in the uniform list do not get removed. 
        sampler.uniformIndex = m_uniforms.size() - 1u;
        
        m_samplers.add(name, sampler);
      }
    }
  }
  
  bool Shader::SetSystemUniform(const String & _name, GLint _location)
  {
    if (_name == PROJECTION_NAME)
    {
      m_projectionLoc = _location;
      return true;
    }
  
    if (_name == VIEW_NAME)
    {
      m_viewLoc = _location;
      return true;
    }
  
    if (_name == MODEL_NAME)
    {
      m_modelLoc = _location;
      return true;
    }
  
    return false;
  }
  
  void Shader::RetreiveUniformData(std::vector<byte> & _outData)
  {
    if (m_uniformSize == 0)
    {
      _outData.clear();
      return;
    }
    _outData.resize(m_uniformSize);
  
    std::vector<byte> data;
    for (auto & uniform : m_uniforms)
    {
      switch (uniform.type)
      {
        case Type::INT:
        {
          RetreiveUniformValue<int>(uniform, data);
          break;
        }
        case Type::FLOAT:
        {
          RetreiveUniformValue<float>(uniform, data);
          break;
        }
        case Type::VEC2:
        {
          RetreiveUniformValue<glm::vec2>(uniform, data);
          break;
        }
        case Type::VEC3:
        {
          RetreiveUniformValue<glm::vec3>(uniform, data);
          break;
        }
        case Type::VEC4:
        {
          RetreiveUniformValue<glm::vec4>(uniform, data);
          break;
        }
        case Type::MAT3:
        {
          RetreiveUniformValue<glm::mat3>(uniform, data);
          break;
        }
        case Type::MAT4:
        {
          RetreiveUniformValue<glm::mat4>(uniform, data);
          break;
        }
        default:
        {
          debug::LogWarning("Invalid uniform type. " + std::to_string(TypeToOpenGL(uniform.type)));
          break;
        }
      }
  
      if (data.empty()) { continue; }
  
      assert(uniform.offset + data.size() <= _outData.size());
  
      memcpy(&_outData[uniform.offset], &data[0], data.size());
    }
  }
  
  void Shader::setModel(const glm::mat4 & _model)
  {
    if (m_modelLoc >= 0)
    {
      setUniform(m_modelLoc, _model);
    }
  }
  
  void Shader::setView(const glm::mat4 & _view)
  {
    if (m_viewLoc >= 0)
    {
      setUniform(m_viewLoc, _view);
    }
  }
  
  void Shader::setProjection(const glm::mat4 & _projection)
  {
    if (m_projectionLoc >= 0)
    {
      setUniform(m_projectionLoc, _projection);
    }
  }
  
  RenderQueue::Queue Shader::getRenderQueue() const
  {
    return m_queue;
  }
  
  GLint Shader::getUniformLocation(const String & _name) const
  {
    GLCALL(GLint loc = glGetUniformLocation(m_program, _name.c_str()));
    return loc;
  }
  
  GLint Shader::getAttributeLocation(const String & _name) const
  {
    GLCALL(GLint loc = glGetAttribLocation(m_program, _name.c_str()));
    return loc;
  }
  
  GLuint Shader::getUniformBlockIndex(const String & _name) const
  {
    GLCALL(GLuint index = glGetUniformBlockIndex(m_program, _name.c_str()));
    return index;
  }
  
  void Shader::setUniformBlockBinding(GLuint _index, GLuint _bind)
  {
    GLCALL(glUniformBlockBinding(m_program, _index, _bind));
  }
  
  void Shader::setUniform(GLint _location, int _val)
  {
    GLCALL(glUniform1i(_location, _val));
  }
  
  void Shader::setUniform(GLint _location, float _val)
  {
    GLCALL(glUniform1f(_location, _val));
  }
  
  void Shader::setUniform(GLint _location, const glm::vec2& _vec)
  {
    GLCALL(glUniform2fv(_location, 1, glm::value_ptr(_vec)));
  }
  
  void Shader::setUniform(GLint _location, const glm::vec3& _vec)
  {
    GLCALL(glUniform3fv(_location, 1, glm::value_ptr(_vec)));
  }
  
  void Shader::setUniform(GLint _location, const glm::vec4& _vec)
  {
    GLCALL(glUniform4fv(_location, 1, glm::value_ptr(_vec)));
  }
  
  void Shader::setUniform(GLint _location, const glm::mat3& _mat)
  {
    GLCALL(glUniformMatrix3fv(_location, 1, GL_FALSE, glm::value_ptr(_mat)));
  }
  
  void Shader::setUniform(GLint _location, const glm::mat4& _mat)
  {
    GLCALL(glUniformMatrix4fv(_location, 1, GL_FALSE, glm::value_ptr(_mat)));
  }
  
  void Shader::setUniform(GLint _location, Type _type, const void * _data)
  {
    switch (_type)
    {
      case Type::INT:
      {
        setUniform(_location, *reinterpret_cast<const int*>(_data));
        break;
      }
      case Type::FLOAT:
      {
        setUniform(_location, *reinterpret_cast<const float*>(_data));
        break;
      }
      case Type::VEC2:
      {
        setUniform(_location, *reinterpret_cast<const glm::vec2*>(_data));
        break;
      }
      case Type::VEC3:
      {
        setUniform(_location, *reinterpret_cast<const glm::vec3*>(_data));
        break;
      }
      case Type::VEC4:
      {
        setUniform(_location, *reinterpret_cast<const glm::vec4*>(_data));
        break;
      }
      case Type::MAT3:
      {
        setUniform(_location, *reinterpret_cast<const glm::mat3*>(_data));
        break;
      }
      case Type::MAT4:
      {
        setUniform(_location, *reinterpret_cast<const glm::mat4*>(_data));
        break;
      }
      default:
      {
        debug::LogError(
          "Shader Error: invalid uniform type: " + std::to_string(TypeToOpenGL(_type))
          + " in shader " + getName()
        );
        break;
      }
    }
  }
  
  bool Shader::getAttribute(const String & _name, ShaderAttribute * _outAttribute) const
  {
    auto attribute = m_attributes.find(_name);
    if (attribute == m_attributes.mend()) { return false; }
  
    if (_outAttribute != nullptr)
    {
      *_outAttribute = m_attributes[attribute->second];
    }
  
    return true;
  }
  
  bool Shader::getUniform(const String & _name, ShaderUniform * _outUniform) const
  {
    auto uniform = m_uniforms.find(_name);
    if (uniform == m_uniforms.mend()) { return false; }
    
    if (_outUniform != nullptr)
    {
      *_outUniform = m_uniforms[uniform->second];
    }
  
    return true;
  }
  
  bool Shader::getSampler(const String & _name, ShaderSampler * _outSampler) const
  {
    auto sampler = m_samplers.find(_name);
    if (sampler == m_samplers.mend()) { return false; }
  
    if (_outSampler != nullptr)
    {
      *_outSampler = m_samplers[sampler->second];
    }
  
    return true;
  }
  
  void Shader::AddShader(const std::shared_ptr<Shader> & _shader)
  {
    if (!_shader) { return; }
  
    auto & shaders = Graphics::getContext().shaders;
  
    for (size_t i = 0u; i < shaders.size(); ++i)
    {
      if (shaders[i].lock() == _shader)
      {
        return;
      }
    }
  
    shaders.push_back(_shader);
  }
  
  void Shader::RemoveShader()
  {
    auto & shaders = Graphics::getContext().shaders;
    for (size_t i = 0u; i < shaders.size();)
    {
      if (shaders[i].expired() ||
          shaders[i].lock().get() == this)
      {
        shaders.erase(shaders.begin() + i);
      }
      else { ++i; }
    }
  }

} } // engine::graphics