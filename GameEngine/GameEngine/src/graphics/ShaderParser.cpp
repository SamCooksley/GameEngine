#include "stdafx.h"

#include "ShaderParser.h"

#include "utilities\File.h"

#include "debug\Debug.h"

namespace engine { namespace graphics {

  static const std::string ShaderTypeToString(ShaderType _type)
  {
    static const std::array<std::string, static_cast<int>(ShaderType::COUNT)> shaderTypeString = {
      "vertex", "fragment", "geometry"
    };

    size_t index = static_cast<size_t>(_type);
    return (index < shaderTypeString.size()) ? shaderTypeString[index] : "none";
  }

  static ShaderType StringToShaderType(const std::string & _type)
  {
    int count = static_cast<int>(ShaderType::COUNT);
    for (int i = static_cast<int>(ShaderType::START); i < count; ++i)
    {
      ShaderType type = static_cast<ShaderType>(i);
      if (string::AreIEqual(_type, ShaderTypeToString(type)))
      {
        return type;
      }
    }

    return ShaderType::NONE;
  }

  ShaderParser::ShaderParser(const std::string & _name, std::istream & _source) :
    m_name(_name), m_currentFile(_name), 
    m_currentType(ShaderType::NONE), m_currentSource(nullptr)
  {
    ParseSource(_source);
  }

  ShaderParser::~ShaderParser()
  { }

  bool ShaderParser::HasShader(ShaderType _type) const
  {
    return !m_sources[static_cast<int>(_type)].empty();
  }

  const std::string & ShaderParser::GetShaderSource(ShaderType _type) const
  {
    if (!HasShader(_type))
    {
      throw std::out_of_range("Shader does not exist in source");
    }

    return m_sources[static_cast<int>(_type)];
  }

  void ShaderParser::ParseSource(std::istream & _source)
  {
    std::string line;
    int count = 0;

    while (std::getline(_source, line))
    { 
      ++count;

      size_t first = line.find_first_not_of(" \t\r\n");
      if (first == std::string::npos) { continue; }

      if (line[first] == '#')
      {
        Preprocessor(line, count);
        continue;
      }

      if (m_currentSource == nullptr)
      {
        throw std::runtime_error(
          "Invalid shader type target. File: " + m_currentFile + 
          ", Line: " + std::to_string(count)
        );
      }

      *m_currentSource += line + '\n';
    }
  }

  void ShaderParser::Preprocessor(const std::string & _line, int _lineNumber)
  {
    size_t pos = std::string::npos;
    if ((pos = _line.find("#shader")) != std::string::npos)
    {
      pos += 8;

      std::string typeText = _line.substr(pos, pos - _line.size());
      ShaderType type = StringToShaderType(typeText);

      if (type == ShaderType::NONE)
      {
        m_currentSource = nullptr;
        m_currentType = ShaderType::NONE;

        throw std::runtime_error(
          "Invalid shader type. " + _line +
          " File: " + m_currentFile + ", Line: " + std::to_string(_lineNumber) +
          "Actual: " + typeText
        );
      }

      SetCurrentShaderType(type);
      return;
    }
    else if ((pos = _line.find("#include")) != std::string::npos)
    {
      std::string lastFile = m_currentFile;
      std::string dir = utilities::file::getDirectory(lastFile);

      size_t start = _line.find('"', pos + 9);
      size_t end = _line.find('"', start + 1);

      if (start == std::string::npos || end == std::string::npos)
      {
        throw std::runtime_error(
          "Failed to find file. " + _line + 
          " File: " + m_name + ", Line: " + std::to_string(_lineNumber)
        );
      }

      std::string path = dir + _line.substr(start + 1, end - start - 1);
      std::ifstream file(path);

      ParseSource(file);

      m_currentFile = lastFile;
      return;
    }
  }

  void ShaderParser::SetCurrentShaderType(ShaderType _type)
  {
    m_currentType = _type;
    size_t index = static_cast<size_t>(_type);
    if (index >= m_sources.size())
    {
      m_currentSource = nullptr;
      throw std::out_of_range("Invalid shader type: " + std::to_string(index));
    }
    m_currentSource = &m_sources[index];
  }

} }