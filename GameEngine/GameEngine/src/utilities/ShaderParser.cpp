#include "stdafx.h"

#include "ShaderParser.h"

#include "File.h"

namespace engine
{
  namespace file
  {
    using ShaderType = graphics::ShaderType;

    ShaderParser::ShaderParser(const std::string & _path) :
      m_name(), m_currentFile(),
      m_currentType(ShaderType::None), m_currentSource(nullptr)
    {
      m_name = file::getFilenameWithoutExtension(_path);
      std::ifstream file(_path);
      if (!file.is_open())
      {
        throw std::runtime_error(
          "ShaderParser Error: " + m_name + " failed to open file: " + _path
        );
      }
      m_currentFile = _path;

      ParseSource(file);
    }

    ShaderParser::ShaderParser(const std::string & _name, std::istream & _source) :
      m_name(_name), m_currentFile(),
      m_currentType(ShaderType::None), m_currentSource(nullptr)
    {
      ParseSource(_source);
    }

    ShaderParser::~ShaderParser()
    { }

    const std::string & ShaderParser::getName() const
    {
      return m_name;
    }

    bool ShaderParser::HasShader(ShaderType::Type _type) const
    {
      return !m_sources[_type].empty();
    }

    const std::string & ShaderParser::getShaderSource(ShaderType::Type _type) const
    {
      return m_sources[_type];
    }

    void ShaderParser::ParseSource(std::istream & _source)
    {
      std::string line;
      int lineNo = 0;

      while (std::getline(_source, line))
      {
        ++lineNo;

        size_t first = line.find_first_not_of(" \t\r\n");
        if (first == std::string::npos) { continue; }

        if (line[first] == '#')
        {
          if (!Preprocessor(line, lineNo))
          {
            continue;
          }
        }

        if (m_currentSource == nullptr)
        {
          throw std::runtime_error(
            "ShaderParser Error: " + m_name + " invalid shader type target. File: " +
            m_currentFile + ", Line: " + std::to_string(lineNo) + ", " + line
          );
        }

        *m_currentSource += line + '\n';
      }
    }

    bool ShaderParser::Preprocessor(const std::string & _line, int _lineNumber)
    {
      size_t pos = std::string::npos;
      if ((pos = _line.find("#shader")) != std::string::npos)
      {
        pos += 8;

        std::string typeText = _line.substr(pos, pos - _line.size());
        ShaderType::Type type = ShaderType::FromString(typeText);

        if (type == ShaderType::None)
        {
          m_currentSource = nullptr;
          m_currentType = ShaderType::None;

          throw std::runtime_error(
            "ShaderParser Error: " + m_name + " invalid shader type " + typeText +
            ". File: " + m_currentFile + ", Line: " + std::to_string(_lineNumber) +
            ", " + _line
          );
        }

        SetCurrentShaderType(type);
        return false;
      }
      else if ((pos = _line.find("#include")) != std::string::npos)
      {
        std::string lastFile = m_currentFile;
        std::string dir = file::getDirectory(lastFile);

        size_t start = _line.find('"', pos + 9);
        size_t end = _line.find('"', start + 1);

        if (start == std::string::npos || end == std::string::npos)
        {
          throw std::runtime_error(
            "ShaderParser Error: " + m_name + " failed to find file name. File: "
            + m_currentFile + ", Line: " + std::to_string(_lineNumber) + ", " + _line
          );
        }

        std::string path = dir + _line.substr(start + 1, end - start - 1);
        std::ifstream file(path);

        if (!file.is_open())
        {
          throw std::runtime_error(
            "ShaderParser Error: " + m_name + " failed to open file " + path +
            ". File: " + m_currentFile + ", Line: " + std::to_string(_lineNumber) +
            ", " + _line
          );
        }
        m_currentFile = path;

        ParseSource(file);

        m_currentFile = lastFile;
        return false;
      }

      return true;
    }

    void ShaderParser::SetCurrentShaderType(ShaderType::Type _type)
    {
      if (_type < 0 || static_cast<size_t>(_type) >= m_sources.size())
      {
        m_currentType = ShaderType::None;
        m_currentSource = nullptr;
        return;
      }

      m_currentType = _type;
      m_currentSource = &m_sources[_type];
    }
  }
}