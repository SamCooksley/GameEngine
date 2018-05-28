#ifndef _ENGINE_UTILITIES_SHADERPARSER_H_
#define _ENGINE_UTILITIES_SHADERPARSER_H_

#include "graphics\ShaderType.h"

#include "graphics\GLData.h"

namespace engine 
{ 
  namespace file
  {
    class ShaderParser
    {
    public:
      ShaderParser(const std::string & _path);
      ShaderParser(const std::string & _name, std::istream & _source);
      ~ShaderParser();

      const std::string & getName() const;

      bool HasShader(graphics::ShaderType::Type _type) const;

      const std::string & getShaderSource(graphics::ShaderType::Type _type) const;

      graphics::Depth::Func getDepth() const;
      graphics::Cull::Face getCull() const;
      graphics::Blend getBlend() const;

    private:
      void ParseSource(std::istream & _source);
      bool Preprocessor(const std::string & _line, int _lineNumber);

      void SetShader(const std::string & _params);
      void Include(const std::string & _params);

      void SetDepth(const std::string & _params);
      void SetCull(const std::string & _params);
      void SetBlend(const std::string & _params);

      void SetCurrentShaderType(graphics::ShaderType::Type _type);

      std::string m_name;

      std::string m_currentFile;

      graphics::ShaderType::Type m_currentType;
      std::string * m_currentSource;
      std::array<std::string, graphics::ShaderType::COUNT> m_sources;

      graphics::Depth::Func m_depth;
      graphics::Cull::Face m_cull;
      graphics::Blend m_blend;
    };

  } 
}

#endif //_ENGINE_UTILITIES_SHADERPARSER_H_