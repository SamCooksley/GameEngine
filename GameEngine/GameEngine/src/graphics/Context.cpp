#include "stdafx.h"

#include "Context.h"

namespace engine {

  void Destroy(std::unique_ptr<graphics::Context> & _context)
  {
    if (!_context) { return; }

    _context->defaultFrameBuffer = nullptr;
    _context->errorShader = nullptr;
    _context->defaultMaterial = nullptr;
    _context->defaultRenderer = nullptr;
    _context->screenQuad = nullptr;

    _context = nullptr;
  }

} // engine