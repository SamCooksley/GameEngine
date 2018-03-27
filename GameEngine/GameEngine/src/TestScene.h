#ifndef _ENGINE_TESTSCENE_H_
#define _ENGINE_TESTSCENE_H_

#include "Scene.h"

namespace engine
{
  class TestScene : public Scene
  {
  public:
    TestScene();
    ~TestScene();

  protected:
    void Init() override;
  };
}

#endif //_ENGINE_TESTSCENE_H_