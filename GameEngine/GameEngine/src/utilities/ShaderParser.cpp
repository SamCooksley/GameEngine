#include "stdafx.h"

#include "ShaderParser.h"

#include "File.h"

namespace engine
{
  namespace file
  {
    using namespace graphics; 

    ShaderParser::ShaderParser(const std::string & _path) :
      m_name(), m_currentFile(),
      m_currentType(ShaderType::NONE), m_currentSource(nullptr),
      m_depth(Depth::LESS), m_cull(Cull::BACK), m_blend(Blend::Disable())
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
      m_currentType(ShaderType::NONE), m_currentSource(nullptr),
      m_depth(graphics::Depth::LESS), m_cull(graphics::Cull::BACK), m_blend(graphics::Blend::Disable())
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

    Depth::Func ShaderParser::getDepth() const
    {
      return m_depth;
    }

    Cull::Face ShaderParser::getCull() const
    {
      return m_cull;
    }

    Blend ShaderParser::getBlend() const
    {
      return m_blend;
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
        SetShader(_line.substr(pos, pos - _line.size()));
        return false;
      }
      else if ((pos = _line.find("#include")) != std::string::npos)
      {
        pos += 9;
        Include(_line.substr(pos, pos - _line.size()));
        return false;
      }
      else if ((pos = _line.find("#depth")) != std::string::npos)
      {
        pos += 7;
        SetDepth(_line.substr(pos, pos - _line.size()));
        return false;
      }
      else if ((pos = _line.find("#cull")) != std::string::npos)
      {
        pos += 6;
        SetCull(_line.substr(pos, pos - _line.size()));
        return false;
      }
      else if ((pos = _line.find("#blend")) != std::string::npos)
      {
        pos += 7;
        SetBlend(_line.substr(pos, pos - _line.size()));
        return false;
      }

      return true;
    }

    void ShaderParser::SetShader(const std::string & _params)
    {
      std::vector<std::string> params;
      string::SplitStringWhitespace(_params, params);
      if (params.size() != 1)
      {
        throw std::runtime_error("Invalid shader arguments");
      }

      ShaderType::Type type = ShaderType::FromString(params[0]);

      if (type == ShaderType::NONE)
      {
        m_currentSource = nullptr;
        m_currentType = ShaderType::NONE;
        throw std::runtime_error("Invalid shader type");
      }

      SetCurrentShaderType(type);
    }

    void ShaderParser::Include(const std::string & _params)
    {
      std::string lastFile = m_currentFile;
      std::string dir = file::getDirectory(lastFile);

      size_t start = _params.find('"', 0);
      size_t end = _params.find('"', start + 1);

      if (start == std::string::npos || end == std::string::npos)
      {
        throw std::runtime_error("Failed to find path");;
      }

      std::string path = dir + _params.substr(start + 1, end - start - 1);
      std::ifstream file(path);

      if (!file.is_open())
      {
        throw std::runtime_error("Failed to open file");
      }
      m_currentFile = path;

      ParseSource(file);

      m_currentFile = lastFile;
    }

    void ShaderParser::SetDepth(const std::string & _params)
    {
      std::vector<std::string> params;
      string::SplitStringWhitespace(_params, params);
      if (params.size() != 1)
      {
        throw std::runtime_error("Invalid depth arguments");
      }

      m_depth = Depth::FromString(params[0]);
    }

    void ShaderParser::SetCull(const std::string & _params)
    {
      std::vector<std::string> params;
      string::SplitStringWhitespace(_params, params);
      if (params.size() != 1)
      {
        throw std::runtime_error("Invalid cull arguments");
      }

      m_cull = Cull::FromString(params[0]);
    }

    void ShaderParser::SetBlend(const std::string & _params)
    {
      std::vector<std::string> params;
      string::SplitStringWhitespace(_params, params);
      if (params.size() == 1 && string::AreIEqual(params[0], "none"))
      {
        m_blend.enable = false;
      }
      else if (params.size() >= 2 && params.size() <= 3)
      {
        m_blend.enable = true;

        m_blend.src = BlendFactor::FromString(params[0]);
        m_blend.dst = BlendFactor::FromString(params[1]);

        if (params.size() > 2)
        {
          m_blend.op = BlendOp::FromString(params[2]);
        }
        else
        {
          m_blend.op = BlendOp::ADD;
        }
      }
      else
      {
        throw std::runtime_error("Invalid cull arguments");
      }
    }

    void ShaderParser::SetCurrentShaderType(ShaderType::Type _type)
    {
      if (_type < 0 || static_cast<size_t>(_type) >= m_sources.size())
      {
        m_currentType = ShaderType::NONE;
        m_currentSource = nullptr;
        return;
      }

      m_currentType = _type;
      m_currentSource = &m_sources[_type];
    }
  }
}