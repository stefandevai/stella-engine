#pragma once

#include "stella/graphics/shader.hpp"
#include "sprite_layer.hpp"

namespace stella
{
namespace graphics
{
  class FogLayer : public SpriteLayer
  {
  public:
    FogLayer (float width, float height, const char* vspath, const char* fspath, bool fixed = true);
    ~FogLayer();
  };
} // namespace graphics
} // namespace stella
