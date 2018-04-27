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
      int count;
      int unit;
      std::shared_ptr<Texture> texture;
    };

    class Material : public Object
    {
      friend class Shader;
    public:
      enum class Error { OK, INVALID_UNIFORM, INVALID_TYPE, INVALID_TEXTURE_UNIT };

      static std::shared_ptr<Material> Create(std::shared_ptr<Shader> _shader);
      static std::shared_ptr<Material> Create(std::shared_ptr<Material> _material);

      ~Material();

      void Bind() const;
      void Unbind() const;

      template <typename T>
      Error setUniform(const std::string & _name, const T & _value);

      template <typename T>
      Error getUniform(const std::string & _name, T * _outValue) const;

      Error setTexture(const std::string & _name, std::shared_ptr<Texture2D> _texture, bool _new = true);
      Error setTexture(const std::string & _name, std::shared_ptr<TextureCube> _textureCube, bool _new = true);

      Error setTextureUnit(const std::string & _name, int _unit);
      Error getTextureUnit(const std::string & _name, int * _unit) const;

      Error getTexture(const std::string & _name, std::shared_ptr<Texture2D> * _outTexture) const;
      Error getTexture(const std::string & _name, std::shared_ptr<TextureCube> * _outTextureCube) const;
      
      std::shared_ptr<Shader> getShader();

    protected:
      static std::shared_ptr<Material> CreateInternal(std::shared_ptr<Shader> _shader);
      Material();

    private:
      template <typename T>
      Error setUniform(const ShaderUniform & _uniform, const T & _value);
      template <typename T>
      Error getUniform(const ShaderUniform & _uniform, T * _outValue) const;

      Error setTexture(const ShaderSampler & _sampler, TextureType _type, std::shared_ptr<Texture> _texture, bool _new = true);

      Error setTextureUnit(const ShaderSampler & _sampler, int _unit);
      Error getTextureUnit(const ShaderSampler & _sampler, int * _unit) const;

      int FindFreeTextureUnit() const;

      std::shared_ptr<Shader> m_shader;

      std::vector<byte> m_uniformData;
      Dictionary<int, TextureUnit> m_textures;
    };
  }
}

#include "Material.inl"

#endif //_ENGINE_GRAPHICS_MATERIAL_H_