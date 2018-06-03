#ifndef _ENGINE_GRAPHICS_GLDATA_H_
#define _ENGINE_GRAPHICS_GLDATA_H_

#include "opengl.h"

#include "GLType.h"

namespace engine
{
  namespace graphics
  {
    class Depth
    {
    public:
      Depth() = delete;

      enum Func
      {
        NONE, 
        NEVER,  ALWAYS,
        LESS,   GREATER,
        EQUAL,  NOTEQUAL,
        LEQUAL, GEQUAL,

        COUNT
      };

      static GLenum ToOpenGL(Func _depth);
      static Func FromOpenGL(GLenum _dept);

      static const std::string & ToString(Func _depth);
      static Func FromString(const std::string & _s);

    private:
      static const std::array<std::string, COUNT> s_names;
    };

    class Cull
    {
    public:
      Cull() = delete;

      enum Face
      {
        NONE,
        FRONT, BACK,

        COUNT
      };

      static GLenum ToOpenGL(Face _cull);
      static Face FromOpenGL(GLenum _cull);

      static const std::string & ToString(Face _cull);
      static Face FromString(const std::string & _s);

    private:
      static const std::array<std::string, COUNT> s_names;
    };

    class BlendFactor
    {
    public:
      BlendFactor() = delete;

      enum Factor
      {
        NONE,
        ZERO, ONE,
        SRC_COLOUR, ONE_MINUS_SRC_COLOUR,
        SRC_ALPHA,  ONE_MINUS_SRC_ALPHA,
        DST_COLOUR, ONE_MINUS_DST_COLOUR,
        DST_ALPHA,  ONE_MINUS_DST_ALPHA,

        COUNT
      };

      static GLenum ToOpenGL(Factor _blend);
      static Factor FromOpenGL(GLenum _blend);

      static const std::string & ToString(Factor _blend);
      static Factor FromString(const std::string & _s);

    private:
      static const std::array<std::string, COUNT> s_names;
    };

    class BlendOp
    {
    public:
      BlendOp() = delete;

      enum Op
      {
        NONE,
        ADD,
        SUB, REV_SUB,
        MIN, MAX,

        COUNT
      };

      static GLenum ToOpenGL(Op _op);
      static Op FromOpenGL(GLenum _op);

      static const std::string & ToString(Op _op);
      static Op FromString(const std::string & _s);

    private:
      static const std::array<std::string, COUNT + 1> s_names;
    };

    struct Blend
    {
      bool enable;
      BlendFactor::Factor src;
      BlendFactor::Factor dst;
      BlendOp::Op op;

      static Blend Disable();
    };

    class PolygonMode
    {
    public:
      PolygonMode() = delete;

      enum Mode
      {
        POINT, LINE, FILL,

        COUNT
      };

      static GLenum ToOpenGL(Mode _mode);
      static Mode FromOpenGL(GLenum _mode);

      static const std::string & ToString(Mode _mode);
      static Mode FromString(const std::string & _s);

    private:
      static const std::array<std::string, COUNT> s_names;
    };

    class GLData : public NonCopyable
    {
    public:
      GLData();
      ~GLData();

      void SetDepthWrite(bool _write);
      void SetDepth(Depth::Func _depth);
      void SetCull(Cull::Face _cull);
      void SetPolygonMode(PolygonMode::Mode _mode);

      void EnableBlend(BlendFactor::Factor _src, BlendFactor::Factor _dst, BlendOp::Op _op = BlendOp::ADD);
      void DisableBlend();

      void SetBlend(const Blend & _blend);

      uint GetMaxUniformBuffers() const;
      uint GetMaxColourAttachments() const;

    private:
      bool m_depthWrite;
      Depth::Func m_depth;
      Cull::Face m_cull;
      PolygonMode::Mode m_polygonMode;
      Blend m_blend;

      uint m_maxUniformBuffers;
      uint m_maxColourAttachments;
    };
  }
}

#endif //_ENGINE_GRAPHICS_GLDATA_H_