#ifndef _ENGINE_UTILITIES_SHADERPARSER_H_
#define _ENGINE_UTILITIES_SHADERPARSER_H_

#include "graphics\ShaderType.h"

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

    private:
      void ParseSource(std::istream & _source);
      std::string Preprocessor(const std::string & _line, int _lineNumber);

      void SetCurrentShaderType(graphics::ShaderType::Type _type);

      std::string m_name;

      std::string m_currentFile;

      graphics::ShaderType::Type m_currentType;
      std::string * m_currentSource;
      std::array<std::string, graphics::ShaderType::Count> m_sources;
    };

  } 
}

#endif //_ENGINE_UTILITIES_SHADERPARSER_H_