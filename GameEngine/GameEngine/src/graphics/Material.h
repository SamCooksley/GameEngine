#ifndef _ENGINE_GRAPHICS_MATERIAL_H_
#define _ENGINE_GRAPHICS_MATERIAL_H_

#include "Shader.h"

namespace engine
{
  namespace graphics
  {
    class Material : public core::Object
    {
      friend class Shader;
    public:
      static std::shared_ptr<Material> Create(std::shared_ptr<Shader> _shader);
      static std::shared_ptr<Material> Create(std::shared_ptr<Material> _material);

      ~Material();

      void Bind() const;
      void Unbind() const;

      template <typename T>
      void setUniform(const std::string & _name, const T & _value);

      template <typename T>
      bool getUniform(const std::string & _name, T * _outValue) const;

      //void setTexture(const std::string & _name, const std::shared_ptr<Texture> _texture);
      //std::shared_ptr<Texture> getTexture(const std::string & _name) const;

      std::shared_ptr<Shader> getShader();

    protected:
      Material();

    private:
      void setUniform(const std::string & _name, const void * _data, uint _size, GLenum _type);
      bool getUniform(const std::string & _name, void * _outValue, uint _size, GLenum _type) const;

      void setShader(std::shared_ptr<Shader> _shader);
      void RemoveFromShader();

      void UpdateSizes();
      void Reset();

      std::shared_ptr<Shader> m_shader;

      std::vector<byte> m_uniformData;
    };
  }
}

#include "Material.inl"

#endif //_ENGINE_GRAPHICS_MATERIAL_H_