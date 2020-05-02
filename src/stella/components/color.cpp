#include "stella/components/color.hpp"

#include <algorithm>
#include <sstream>

namespace stella
{
namespace component
{
  Color::Color (const std::string& color) : hex_color (color)
  {
    std::stringstream ss;
    std::string only_hex_str{hex_color.substr (1)};
    std::reverse (only_hex_str.begin(), only_hex_str.end());
    ss << std::hex << only_hex_str;
    ss >> int_color;

    rgba_color.a = (int_color & 0xFF000000) >> 24;
    rgba_color.b = (int_color & 0x00FF0000) >> 16;
    rgba_color.g = (int_color & 0x0000FF00) >> 8;
    rgba_color.r = (int_color & 0x000000FF);
  }

  Color::Color (const unsigned r, const unsigned g, const unsigned b, const unsigned a)
  {
    std::stringstream ss;

    rgba_color.r = r;
    rgba_color.g = g;
    rgba_color.b = b;
    rgba_color.a = a;
    int_color    = a << 24 | b << 16 | g << 8 | r;
    ss << '#' << std::hex << int_color;
    hex_color = ss.str();
  }
} // namespace component
} // namespace stella