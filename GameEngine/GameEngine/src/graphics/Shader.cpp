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
        shader->SetupSystemUniforms();
      }
      catch (std::exception & _e)
      {
        debug::LogError(_e.what());
        return Graphics::getErrorShader();
      }

      return shader;
    }

    Shader::Shader()
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

    bool Shader::AddUniform(const std::string & _name, GLenum _type, uint _size, ShaderUniform * _outUniform)
    {
      if (getUniform(_name, _outUniform)) { return true; }

      int location = getUniformLocation(_name);
      if (location < 0)
      {
        debug::LogError("Shader Error: " + getName() + " does not contain uniform: " + _name);
        return false;
      }

      ShaderUniform uniform = {
        _name,
        _type, 
        location,
        _size,
        m_uniformSize
      };

      m_uniforms.add(_name, uniform);

      ResizeUniformBuffer(m_uniformSize + _size);

      if (_outUniform != nullptr)
      {
        *_outUniform = uniform;
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

    bool Shader::AddTexture(const std::string & _name, ShaderTexture * _outTexture)
    {
      if (getTexture(_name, _outTexture)) { return true; }

      GLint location = getUniformLocation(_name);

      if (location < 0)
      {
        debug::LogError("Shader Error: " + getName() + " does not contain texture uniform: " + _name);
        return false;
      }

      ShaderTexture texture = {
        _name,
        m_textures.size(),
        location
      };

      m_textures.add(_name, texture);

      UpdateMaterials();

      if (_outTexture != nullptr)
      {
        *_outTexture = texture;
      }
      return true;
    }

    bool Shader::getTexture(const std::string & _name, ShaderTexture * _outTexture) const
    {
      auto texture = m_textures.find(_name);
      if (texture == m_textures.mend()) { return false; }

      if (_outTexture != nullptr)
      {
        *_outTexture = m_textures[texture->second];
      }

      return true;
    }

    bool Shader::AddAttribute(const std::string & _name, GLenum _type, uint _count, bool _normalized, ShaderAttribute * _outAttribute)
    {
      if (getAttribute(_name, _outAttribute)) { return true; }

      GLint location = getAttributeLocation(_name);
      if (location < 0) { return false; }

      ShaderAttribute attr = {
        _name,
        _type,
        _count,
        _normalized,
        location
      };

      m_attributes.add(_name, attr);

      if (_outAttribute != nullptr)
      {
        *_outAttribute = attr;
      }

      return true;
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

    void Shader::ResizeUniformBuffer(size_t _size)
    {
      m_uniformSize = _size;
      UpdateMaterials();
    }

    void Shader::UpdateMaterials()
    {
      for (auto & mat : m_materials)
      {
        mat->UpdateSizes();
      }
    }

    void Shader::AddMaterial(Material * _material)
    {
      for (auto & mat : m_materials)
      {
        if (mat == _material) { return; }
      }

      m_materials.push_back(_material);
    }

    void Shader::RemoveMaterial(Material * _material)
    {
      for (size_t i = 0; i < m_materials.size();)
      {
        if (m_materials[i] == _material)
        {
          m_materials.erase(m_materials.begin() + i);
        }
        else { ++i; }
      }
    }
  }
}