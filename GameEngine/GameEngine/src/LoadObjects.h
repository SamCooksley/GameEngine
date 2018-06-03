#ifndef _ENGINE_LOADOBJECTS_H_
#define _ENGINE_LOADOBJECTS_H_

#include "GameObject.h"
#include "graphics\Graphics.h"

namespace engine {

  std::shared_ptr<GameObject> Load(const String & _path, std::shared_ptr<graphics::Material> _baseMaterial = Graphics::getDefaultMaterial());

} // engine

#endif //_ENGINE_LOADOBJECTS_H_