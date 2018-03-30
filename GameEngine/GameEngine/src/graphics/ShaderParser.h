#ifndef _ENGINE_GRAPHICS_SHADERPARSER_H_
#define _ENGINE_GRAPHICS_SHADERPARSER_H_

#include "core\Types.h"

#include "ShaderType.h"

namespace engine 
{ 
  namespace graphics
  {

    class ShaderParser
    {
    public:
      ShaderParser(const std::string & _path);
      ShaderParser(const std::string & _name, std::istream & _source);
      ~ShaderParser();

      const std::string & getName() const;

      bool HasShader(ShaderType::Type _type) const;

      const std::string & getShaderSource(ShaderType::Type _type) const;

    private:
      void ParseSource(std::istream & _source);
      std::string Preprocessor(const std::string & _line, int _lineNumber);

      void SetCurrentShaderType(ShaderType::Type _type);

      std::string m_name;

      std::string m_currentFile;

      ShaderType::Type m_currentType;
      std::string * m_currentSource;
      std::array<std::string, ShaderType::Count> m_sources;
    };

  } 
}

#endif //_ENGINE_GRAPHICS_SHADERPARSER_H_