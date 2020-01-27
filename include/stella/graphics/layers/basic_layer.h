#pragma once

#include "../shader.h"
#include "./layer.h"

namespace stella
{
namespace graphics
{
  class BasicLayer : public Layer
  {
  public:
    BasicLayer (float width, float height, const char* vspath, const char* fspath, bool fixed = true);
    ~BasicLayer();
  };
} // namespace graphics
} // namespace stella
