#include "stdafx.h"

#include "Shader.h"

#include "Material.h"

#include "utilities\ShaderParser.h"

#include "Graphics.h"

namespace engine 
{
  namespace graphics 
  {
    const std::string Shader::PROJECTION_NAME = "projection";
    const std::string Shader::VIEW_NAME       = "view";
    const std::string Shader::MODEL_NAME      = "model";

    const std::string Shader::ATTR_POSITION_NAME  = "in_position";
    const std::string Shader::ATTR_UV_NAME        = "in_texCoords";
    const std::string Shader::ATTR_NORMAL_NAME    = "in_normal";
    const std::string Shader::ATTR_TANGENT_NAME   = "in_tangent";
    const std::string Shader::ATTR_BITANGENT_NAME = "in_bitangent";

    std::shared_ptr<Shader> Shader::Load(const std::string & _path)
    {
      class enable_shader : public Shader { };

      std::shared_ptr<Shader> shader;

      try
      {
        file::ShaderParser parser(_path);
        shader = std::make_shared<enable_shader>();
        shader->setName(parser.getName());

        for (ShaderType::Type i = 0; i < ShaderType::Count; ++i)
        {
          if (parser.HasShader(i))
          {
            shader->CreateShader(parser.getShaderSource(i), i);
          }
        }
        shader->LinkProgram();
        shader->DetachAndDeleteShaders();

        shader->SetupUniformBuffers();
        shader->SetupAttributes();
        shader->SetupUniforms();
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
      m_modelLoc(-1), m_viewLoc(-1), m_projectionLoc(-1)
    {
      GLCALL(m_program = glCreateProgram());
    }

    Shader::~Shader()
    {
      GLCALL(glDeleteProgram(m_program));
    }

    void Shader::Bind() const
    {
      GLCALL(glUseProgram(m_program));
    }

    void Shader::Unbind() const
    {
      GLCALL(glUseProgram(0));
    }

    void Shader::CreateShader(const std::string & _source, ShaderType::Type _type)
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
          "Shader Error: " + getName() + " compilation failed. " + std::string(&log[0], length)
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
          "Shader Error: " + getName() + " linking failed. " + std::string(&log[0], length)
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
        GLuint index = getUniformBlockIndex(buffer.first);
        if (index != GL_INVALID_INDEX)
        {
          setUniformBlockBinding(index, buffer.second->getBind());
        }
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

        std::string name(nameBuffer.begin(), nameBuffer.begin() + length);

        GLint loc = getAttributeLocation(name);
        
        if (loc < 0) { continue; }
        
        ShaderAttribute attribute;
        attribute.name = name;
        attribute.type = type;
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

        std::string name(nameBuffer.begin(), nameBuffer.begin() + length);

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

        bool isSampler = IsSamplerUniformType(type);

        ShaderUniform uniform;
        uniform.name = name;
        uniform.type = isSampler ? GL_INT : type;
        uniform.size = GetTypeSize(uniform.type);
        uniform.location = loc;

        uniform.offset = m_uniformSize;
        m_uniformSize += uniform.size;

        m_uniforms.add(name, uniform);

        if (isSampler)
        {
          ShaderSampler sampler;
          sampler.name = name;
          sampler.type = type;
          //should be okay as elements in the uniform list do not get removed. 
          sampler.uniformIndex = m_uniforms.size() - 1u;
          
          m_samplers.add(name, sampler);
        }
      }
    }

    bool Shader::SetSystemUniform(const std::string & _name, GLint _location)
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
      _outData.resize(m_uniformSize);

      std::vector<byte> data;
      for (auto & uniform : m_uniforms)
      {
        switch (uniform.type)
        {
          case GL_INT:
          {
            RetreiveUniformValue<int>(uniform, data);
            break;
          }
          case GL_FLOAT:
          {
            RetreiveUniformValue<float>(uniform, data);
            break;
          }
          case GL_FLOAT_VEC2:
          {
            RetreiveUniformValue<glm::vec2>(uniform, data);
            break;
          }
          case GL_FLOAT_VEC3:
          {
            RetreiveUniformValue<glm::vec3>(uniform, data);
            break;
          }
          case GL_FLOAT_VEC4:
          {
            RetreiveUniformValue<glm::vec4>(uniform, data);
            break;
          }
          case GL_FLOAT_MAT3:
          {
            RetreiveUniformValue<glm::mat3>(uniform, data);
            break;
          }
          case GL_FLOAT_MAT4:
          {
            RetreiveUniformValue<glm::mat4>(uniform, data);
            break;
          }
          default:
          {
            debug::LogWarning("Invalid uniform type. " + std::to_string(uniform.type));
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

    GLint Shader::getUniformLocation(const std::string & _name) const
    {
      GLCALL(GLint loc = glGetUniformLocation(m_program, _name.c_str()));
      return loc;
    }

    GLint Shader::getAttributeLocation(const std::string & _name) const
    {
      GLCALL(GLint loc = glGetAttribLocation(m_program, _name.c_str()));
      return loc;
    }

    GLuint Shader::getUniformBlockIndex(const std::string & _name) const
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

    void Shader::setUniform(GLint _location, GLenum _type, const void * _data)
    {
      switch (_type)
      {
        case GL_INT:
        {
          setUniform(_location, *reinterpret_cast<const int*>(_data));
          break;
        }
        case GL_FLOAT:
        {
          setUniform(_location, *reinterpret_cast<const float*>(_data));
          break;
        }
        case GL_FLOAT_VEC2:
        {
          setUniform(_location, *reinterpret_cast<const glm::vec2*>(_data));
          break;
        }
        case GL_FLOAT_VEC3:
        {
          setUniform(_location, *reinterpret_cast<const glm::vec3*>(_data));
          break;
        }
        case GL_FLOAT_VEC4:
        {
          setUniform(_location, *reinterpret_cast<const glm::vec4*>(_data));
          break;
        }
        case GL_FLOAT_MAT3:
        {
          setUniform(_location, *reinterpret_cast<const glm::mat3*>(_data));
          break;
        }
        case GL_FLOAT_MAT4:
        {
          setUniform(_location, *reinterpret_cast<const glm::mat4*>(_data));
          break;
        }
        default:
        {
          debug::LogError("Shader Error: invalid uniform type: " + std::to_string(_type) + " in shader " + getName());
          break;
        }
      }
    }

    bool Shader::getAttribute(const std::string & _name, ShaderAttribute * _outAttribute) const
    {
      auto attribute = m_attributes.find(_name);
      if (attribute == m_attributes.mend()) { return false; }

      if (_outAttribute != nullptr)
      {
        *_outAttribute = m_attributes[attribute->second];
      }

      return true;
    }

    bool Shader::getUniform(const std::string & _name, ShaderUniform * _outUniform) const
    {
      auto uniform = m_uniforms.find(_name);
      if (uniform == m_uniforms.mend()) { return false; }
      
      if (_outUniform != nullptr)
      {
        *_outUniform = m_uniforms[uniform->second];
      }

      return true;
    }

    bool Shader::getSampler(const std::string & _name, ShaderSampler * _outSampler) const
    {
      auto sampler = m_samplers.find(_name);
      if (sampler == m_samplers.mend()) { return false; }

      if (_outSampler != nullptr)
      {
        *_outSampler = m_samplers[sampler->second];
      }

      return true;
    }
  }
}