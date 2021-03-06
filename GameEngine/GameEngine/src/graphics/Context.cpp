#include "stdafx.h"

#include "Context.h"

namespace engine {

  void Destroy(std::unique_ptr<graphics::Context> & _context)
  {
    if (!_context) { return; }

    _context->defaultFrameBuffer = nullptr;
    _context->captureFBO = nullptr;
    _context->errorShader = nullptr;
    _context->depthShader = nullptr;
    _context->defaultMaterial = nullptr;
    _context->quad = nullptr;

    _context = nullptr;
  }

} // engine