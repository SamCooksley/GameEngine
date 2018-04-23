namespace engine
{
  namespace graphics
  {
    template <typename T>
    void Shader::RetreiveUniformValue(const ShaderUniform & _uniform, std::vector<byte> & _dest) const
    {
      T v = RetreiveUniformValue<T>(_uniform);
      _dest.resize(sizeof(T));
      memcpy(&_dest[0], &v, sizeof(T));
    }

    /*template <typename T>
    T Shader::RetreiveUniformValue<T>(const ShaderUniform & _uniform) const
    {
      static_assert(true, "Unsupported uniform type.");
    }*/

    template<>
    inline int Shader::RetreiveUniformValue<int>(const ShaderUniform & _uniform) const
    {
      if (_uniform.type != GL_INT)
      {
        throw std::invalid_argument(
          "Shader Error: " + getName() + " uniform " + 
          _uniform.name + " is not of type int."
        );
      }

      int data;
      GLCALL(glGetnUniformiv(m_program, _uniform.location, _uniform.size, &data));
      return data;
    }

    template<>
    inline float Shader::RetreiveUniformValue<float>(const ShaderUniform & _uniform) const
    {
      if (_uniform.type != GL_FLOAT)
      {
        throw std::invalid_argument(
          "Shader Error: " + getName() + " uniform " +
          _uniform.name + " is not of type float."
        );
      }

      float data;
      GLCALL(glGetnUniformfv(m_program, _uniform.location, _uniform.size, &data));
      return data;
    }

    template<>
    inline glm::vec2 Shader::RetreiveUniformValue<glm::vec2>(const ShaderUniform & _uniform) const
    {
      if (_uniform.type != GL_FLOAT_VEC2)
      {
        throw std::invalid_argument(
          "Shader Error: " + getName() + " uniform " +
          _uniform.name + " is not of type vec2."
        );
      }

      glm::vec2 data;
      GLCALL(glGetnUniformfv(m_program, _uniform.location, _uniform.size, glm::value_ptr(data)));
      return data;
    }

    template<>
    inline glm::vec3 Shader::RetreiveUniformValue<glm::vec3>(const ShaderUniform & _uniform) const
    {
      if (_uniform.type != GL_FLOAT_VEC3)
      {
        throw std::invalid_argument(
          "Shader Error: " + getName() + " uniform " +
          _uniform.name + " is not of type vec3."
        );
      }

      glm::vec3 data;
      GLCALL(glGetnUniformfv(m_program, _uniform.location, _uniform.size, glm::value_ptr(data)));
      return data;
    }

    template<>
    inline glm::vec4 Shader::RetreiveUniformValue<glm::vec4>(const ShaderUniform & _uniform) const
    {
      if (_uniform.type != GL_FLOAT_VEC4)
      {
        throw std::invalid_argument(
          "Shader Error: " + getName() + " uniform " +
          _uniform.name + " is not of type vec4."
        );
      }

      glm::vec4 data;
      GLCALL(glGetnUniformfv(m_program, _uniform.location, _uniform.size, glm::value_ptr(data)));
      return data;
    }

    template<>
    inline glm::mat3 Shader::RetreiveUniformValue<glm::mat3>(const ShaderUniform & _uniform) const
    {
      if (_uniform.type != GL_FLOAT_MAT3)
      {
        throw std::invalid_argument(
          "Shader Error: " + getName() + " uniform " +
          _uniform.name + " is not of type mat3."
        );
      }

      glm::mat3 data;
      GLCALL(glGetnUniformfv(m_program, _uniform.location, _uniform.size, glm::value_ptr(data)));
      return data;
    }

    template<>
    inline glm::mat4 Shader::RetreiveUniformValue<glm::mat4>(const ShaderUniform & _uniform) const
    {
      if (_uniform.type != GL_FLOAT_MAT4)
      {
        throw std::invalid_argument(
          "Shader Error: " + getName() + " uniform " +
          _uniform.name + " is not of type mat4."
        );
      }

      glm::mat4 data;
      GLCALL(glGetnUniformfv(m_program, _uniform.location, _uniform.size, glm::value_ptr(data)));
      return data;
    }
  }
}