#pragma once

#include "component.hpp"
#include <vector>

namespace stella
{
namespace component
{
  struct SpriteGroup : public Component
  {
    SpriteGroup() : Component ("Sprite Group") {}
    SpriteGroup(const unsigned width, const unsigned height, std::vector<int> sprites)
    : Component ("Sprite Group"), width(width), height(height)
    {}
    unsigned width = 0, height = 0;
  };
} // namespace component
} // namespace stella
