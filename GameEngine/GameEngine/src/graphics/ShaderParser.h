#ifndef _ENGINE_GRAPHICS_SHADERPARSER_H_
#define _ENGINE_GRAPHICS_SHADERPARSER_H_

#include "core\Types.h"

namespace engine { namespace graphics {

  enum class ShaderType
  {
    NONE = -1,
    VERTEX = 0,
    FRAGMENT = 1,
    GEOMETRY = 2, 

    COUNT,
    START = VERTEX
  };

  class ShaderParser
  {
  public:
    ShaderParser(const std::string & _name, std::istream & _source);
    ~ShaderParser();

    bool HasShader(ShaderType _type) const;

    const std::string & GetShaderSource(ShaderType _type) const;

  private:
    void ParseSource(std::istream & _source);
    void Preprocessor(const std::string & _line, int _lineNumber);

    void SetCurrentShaderType(ShaderType _type);

    std::string m_name;

    std::string m_currentFile;

    ShaderType m_currentType;
    std::string * m_currentSource;
    std::array<std::string, static_cast<int>(ShaderType::COUNT)> m_sources;
  };

} }

#endif //_ENGINE_GRAPHICS_SHADERPARSER_H_