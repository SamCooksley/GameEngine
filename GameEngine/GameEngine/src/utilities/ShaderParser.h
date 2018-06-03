#ifndef _ENGINE_UTILITIES_SHADERPARSER_H_
#define _ENGINE_UTILITIES_SHADERPARSER_H_

#include "graphics\Shader.h"

namespace engine { 
namespace file {

  class ShaderParser
  {
   public:
    ShaderParser(const String & _path);
    ShaderParser(const String & _name, std::istream & _source);
    ~ShaderParser();
  
    const String & getName() const;
  
    bool HasShader(graphics::ShaderType::Type _type) const;
  
    const String & getShaderSource(graphics::ShaderType::Type _type) const;
  
    bool getDepthWrite() const;
    graphics::Depth::Func getDepth() const;
    graphics::Cull::Face getCull() const;
    graphics::Blend getBlend() const;
    graphics::RenderQueue::Queue getQueue() const;
  
   private:
    void ParseSource(std::istream & _source);
    bool Preprocessor(const String & _line, int _lineNumber);
  
    void SetShader(const String & _params);
    void Include(const String & _params);
  
    void SetDepthWrite(const String & _params);
    void SetDepth(const String & _params);
    void SetCull(const String & _params);
    void SetBlend(const String & _params);
    void SetQueue(const String & _params);
  
    void SetCurrentShaderType(graphics::ShaderType::Type _type);
  
   private:
    String m_name;
  
    String m_currentFile;
  
    graphics::ShaderType::Type m_currentType;
    String * m_currentSource;
    std::array<String, graphics::ShaderType::COUNT> m_sources;
  
    bool m_depthWrite;
    graphics::Depth::Func m_depth;
    graphics::Cull::Face m_cull;
    graphics::Blend m_blend;
    graphics::RenderQueue::Queue m_queue;
  };
 
} } // engine::file

#endif //_ENGINE_UTILITIES_SHADERPARSER_H_