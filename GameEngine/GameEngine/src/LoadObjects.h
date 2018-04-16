#ifndef _ENGINE_LOADOBJECTS_H_
#define _ENGINE_LOADOBJECTS_H_

#include "graphics\Graphics.h"

#include "GameObject.h"

#include "utilities\File.h"

namespace engine
{
  std::shared_ptr<GameObject> Load(const std::string & _path, std::shared_ptr<graphics::Material> _baseMaterial = Graphics::getDefaultMaterial());
}

#endif //_ENGINE_LOADOBJECTS_H_