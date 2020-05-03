#pragma once

#include <vector>

namespace stella
{
namespace component
{
  struct SpriteGroup
  {
    SpriteGroup(const unsigned width, const unsigned height, std::vector<int> sprites)
    : width(width), height(height)
    {}
    unsigned width, height;
  };
} // namespace component
} // namespace stella
