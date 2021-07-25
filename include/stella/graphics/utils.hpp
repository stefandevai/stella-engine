#pragma once

#include "opengl.hpp"
#include <iostream>
#include <vector>

namespace stella
{
namespace graphics
{
  class Utils
  {
  public:
    static std::vector<int> get_16x9_viewport_coords (const int width, const int height)
    {
      std::vector<int> new_coords{0, width, height};

      // 16/9 = 1.77777. Therefore, we check if the new proportions are greater or
      // lower than that
      if (width / (float) height > 1.78f)
      { // Height is max and width is adjusted
        new_coords[1] = height * 1.77777f;
        new_coords[0] = width - new_coords[1];
      }
      else if (width / (float) height < 1.77f)
      { // Width is max and height is adjusted
        new_coords[2] = (int) width / 1.77f;
        new_coords[0] = height - new_coords[2];
      }
      return new_coords;
    }
  };
} // namespace graphics
} // namespace stella
