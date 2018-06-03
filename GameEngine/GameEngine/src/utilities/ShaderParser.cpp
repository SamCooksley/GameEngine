#include "stdafx.h"

#include "ShaderParser.h"

#include "File.h"

namespace engine {
namespace file {

  using namespace graphics; 
  
  ShaderParser::ShaderParser(const String & _path) :
    m_name(), m_currentFile(),
    m_currentType(ShaderType::NONE), m_currentSource(nullptr),
    m_depthWrite(true), m_depth(Depth::LESS),
    m_cull(Cull::BACK), m_blend(Blend::Disable()),
    m_queue(RenderQueue::FORWARD)
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
  
  ShaderParser::ShaderParser(const String & _name, std::istream & _source) :
    m_name(_name), m_currentFile(),
    m_currentType(ShaderType::NONE), m_currentSource(nullptr),
    m_depthWrite(true), m_depth(graphics::Depth::LESS), 
    m_cull(graphics::Cull::BACK), m_blend(graphics::Blend::Disable()),
    m_queue(RenderQueue::FORWARD)
  {
    ParseSource(_source);
  }
  
  ShaderParser::~ShaderParser()
  { }
  
  const String & ShaderParser::getName() const
  {
    return m_name;
  }
  
  bool ShaderParser::HasShader(ShaderType::Type _type) const
  {
    return !m_sources[_type].empty();
  }
  
  const String & ShaderParser::getShaderSource(ShaderType::Type _type) const
  {
    return m_sources[_type];
  }
  
  bool ShaderParser::getDepthWrite() const
  {
    return m_depthWrite;
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
  
  RenderQueue::Queue ShaderParser::getQueue() const
  {
    return m_queue;
  }
  
  void ShaderParser::ParseSource(std::istream & _source)
  {
    String line;
    int lineNo = 0;
  
    while (std::getline(_source, line))
    {
      ++lineNo;
  
      size_t first = line.find_first_not_of(" \t\r\n");
      if (first == String::npos) { continue; }
  
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
  
  bool ShaderParser::Preprocessor(const String & _line, int _lineNumber)
  {
    size_t pos = String::npos;
    if ((pos = _line.find("#shader")) != String::npos)
    {
      pos += 8;
      SetShader(_line.substr(pos, pos - _line.size()));
      return false;
    }
    else if ((pos = _line.find("#include")) != String::npos)
    {
      pos += 9;
      Include(_line.substr(pos, pos - _line.size()));
      return false;
    }
    else if ((pos = _line.find("#depthwrite")) != String::npos)
    {
      pos += 12;
      SetDepthWrite(_line.substr(pos, pos - _line.size()));
      return false;
    }
    else if ((pos = _line.find("#depth")) != String::npos)
    {
      pos += 7;
      SetDepth(_line.substr(pos, pos - _line.size()));
      return false;
    }
    else if ((pos = _line.find("#cull")) != String::npos)
    {
      pos += 6;
      SetCull(_line.substr(pos, pos - _line.size()));
      return false;
    }
    else if ((pos = _line.find("#blend")) != String::npos)
    {
      pos += 7;
      SetBlend(_line.substr(pos, pos - _line.size()));
      return false;
    }
    else if ((pos = _line.find("#queue")) != String::npos)
    {
      pos += 7;
      SetQueue(_line.substr(pos, pos - _line.size()));
      return false;
    }
    return true;
  }
  
  void ShaderParser::SetShader(const String & _params)
  {
    auto params = string::SplitStringWhitespace(_params);
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
  
  void ShaderParser::Include(const String & _params)
  {
    String lastFile = m_currentFile;
    String dir = file::getDirectory(lastFile);
  
    size_t start = _params.find('"', 0);
    size_t end = _params.find('"', start + 1);
  
    if (start == String::npos || end == String::npos)
    {
      throw std::runtime_error("Failed to find path");;
    }
  
    String path = dir + _params.substr(start + 1, end - start - 1);
    std::ifstream file(path);
  
    if (!file.is_open())
    {
      throw std::runtime_error("Failed to open file");
    }
    m_currentFile = path;
  
    ParseSource(file);
  
    m_currentFile = lastFile;
  }
  
  void ShaderParser::SetDepthWrite(const String & _params)
  {
    auto params = string::SplitStringWhitespace(_params);
    if (params.size() != 1)
    {
      throw std::runtime_error("Invalid depth write arguments");
    }
  
    if (params[0] == "on")
    {
      m_depthWrite = true;
    }
    else if (params[0] == "off")
    {
      m_depthWrite = false;
    }
    else
    {
      debug::Log(params[0]);
      throw std::invalid_argument("Invalid depth write");
    }
  }
  
  void ShaderParser::SetDepth(const String & _params)
  {
    auto params = string::SplitStringWhitespace(_params);
    if (params.size() != 1)
    {
      throw std::runtime_error("Invalid depth arguments");
    }
  
    m_depth = Depth::FromString(params[0]);
  }
  
  void ShaderParser::SetCull(const String & _params)
  {
    auto params = string::SplitStringWhitespace(_params);
    if (params.size() != 1)
    {
      throw std::runtime_error("Invalid cull arguments");
    }
  
    m_cull = Cull::FromString(params[0]);
  }
  
  void ShaderParser::SetBlend(const String & _params)
  {
    auto params = string::SplitStringWhitespace(_params);
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
  
  void ShaderParser::SetQueue(const String & _params)
  {
    auto params = string::SplitStringWhitespace(_params);  
    if (params.size() != 1)
    {
      throw std::runtime_error("Invalid queue arguments");
    }
  
    m_queue = RenderQueue::FromString(params[0]);
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
  
} } // engine::file