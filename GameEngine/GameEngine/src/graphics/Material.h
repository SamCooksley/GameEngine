#ifndef _ENGINE_GRAPHICS_MATERIAL_H_
#define _ENGINE_GRAPHICS_MATERIAL_H_

#include "Shader.h"
#include "Texture2D.h"
#include "TextureCube.h"

namespace engine {
namespace graphics {

  struct TextureUnit
  {
   public:
    int count; // number of uniforms/samplers bound to the unit.
    int unit;
    TextureType type; // also store the type as texture may be null.
    std::shared_ptr<Texture> texture;
  };
  
  class Material : public NamedObject
  {
    friend class Shader;
   public:
    enum class Error { OK, INVALID_UNIFORM, INVALID_TYPE, INVALID_TEXTURE_UNIT };
  
    Material(const std::shared_ptr<Shader> & _shader);
    ~Material();
  
    void Bind() const;
  
    template <typename T>
    Error setUniform(const String & _name, const T & _value);
  
    template <typename T>
    Error getUniform(const String & _name, T * _outValue) const;

    template <class T>
    Error setTexture(const String & _name, const std::shared_ptr<T> & _texture, bool _new = true);
  
    Error setTextureUnit(const String & _name, int _unit);
    Error getTextureUnit(const String & _name, int * _unit) const;
  
    template <class T>
    Error getTexture(const String & _name, std::shared_ptr<T> * _outTexture) const;
  
    int FindFreeTextureUnit() const;

    const std::shared_ptr<Shader> & getShader();
  
   private:
    template <typename T>
    Error setUniform(const ShaderUniform & _uniform, const T & _value);
    template <typename T>
    Error getUniform(const ShaderUniform & _uniform, T * _outValue) const;
  
    Error setTexture(const ShaderSampler & _sampler, TextureType _type, const std::shared_ptr<Texture> & _texture, bool _new = true);
  
    Error setTextureUnit(const ShaderSampler & _sampler, int _unit);
    Error getTextureUnit(const ShaderSampler & _sampler, int * _unit) const;
  
   private:
    std::shared_ptr<Shader> m_shader;
  
    std::vector<byte> m_uniformData;
    Dictionary<int, TextureUnit> m_textures;
  };

} } // engine::graphics

#include "Material.inl"

#endif //_ENGINE_GRAPHICS_MATERIAL_H_