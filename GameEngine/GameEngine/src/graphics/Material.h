#ifndef _ENGINE_GRAPHICS_MATERIAL_H_
#define _ENGINE_GRAPHICS_MATERIAL_H_

#include "Shader.h"
#include "Texture.h"
#include "Texture2D.h"
#include "TextureCube.h"

namespace engine
{
  namespace graphics
  {
    struct TextureUnit
    {
      GLenum type;
      int count;
      int unit;
      std::shared_ptr<Texture> texture;
    };

    class Material : public Object
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

      void setTexture(const std::string & _name, std::shared_ptr<Texture2D> _texture);
      void setTexture(const std::string & _name, std::shared_ptr<TextureCube> _textureCube);

      void setTextureUnit(const std::string & _name, int _unit);
      int getTextureUnit(const std::string & _name) const;

      bool getTexture(const std::string & _name, std::shared_ptr<Texture2D> * _outTexture) const;
      bool getTexture(const std::string & _name, std::shared_ptr<TextureCube> * _outTextureCube) const;
      
      std::shared_ptr<Shader> getShader();

    protected:
      static std::shared_ptr<Material> CreateInternal(std::shared_ptr<Shader> _shader);
      Material();

    private:
      void setUniform(const std::string & _name, const void * _data, uint _size, GLenum _type);
      bool getUniform(const std::string & _name, void * _outValue, uint _size, GLenum _type) const;

      void setTexture(const std::string & _name, GLenum _type, std::shared_ptr<Texture> _texture);

      bool getTextureInfo(int _unit, TextureUnit * _outTexture) const;

      int FindFreeTextureUnit() const;

      std::shared_ptr<Shader> m_shader;

      std::vector<byte> m_uniformData;
      Dictionary<int, TextureUnit> m_textures;
    };
  }
}

#include "Material.inl"

#endif //_ENGINE_GRAPHICS_MATERIAL_H_