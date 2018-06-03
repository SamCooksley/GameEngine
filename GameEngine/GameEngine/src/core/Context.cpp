#include "stdafx.h"

#include "Context.h"

namespace engine
{
  void Destroy(std::unique_ptr<core::Context> & _context)
  {
    if (!_context) { return; }

    _context->scene = nullptr;
    _context->nextScene = nullptr;

    _context->resources.clear();

    Destroy(_context->graphics);

    _context = nullptr;
  }
}