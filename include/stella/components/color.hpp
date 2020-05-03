#pragma once

#include <string>

namespace stella
{
namespace component
{
  struct Color
  {
  private:
    struct rgba_color_info
    {
      int r, g, b, a;
    };

  public:
    Color (const std::string& color);
    Color (const unsigned r, const unsigned g, const unsigned b, const unsigned a = 255);

    std::string hex_color;
    unsigned int_color;
    rgba_color_info rgba_color;
  };
} // namespace component
} // namespace stella
