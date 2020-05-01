#pragma once

#include "stella/graphics/shader.hpp"
#include "sprite_layer.hpp"

namespace stella
{
namespace graphics
{
  class BasicLayer : public SpriteLayer
  {
  public:
    BasicLayer (float width, float height, const char* vspath, const char* fspath, bool fixed = true);
    ~BasicLayer();
  };
} // namespace graphics
} // namespace stella
