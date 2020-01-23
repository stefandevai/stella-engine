#pragma once

#include <string>
#include <vector>

#include <entt/entity/registry.hpp>

namespace stella {
namespace components {

struct TextComponent {
  TextComponent(const std::wstring text, const std::string font_name, const std::string color = "#ffffffff", bool is_static = false)
    : Text(text), FontName(font_name), color(color), IsStatic(is_static)
  {
    
  }
  std::wstring Text;
  std::string FontName, color;
  std::vector<entt::registry::entity_type> char_entities;
  bool InLayer = false, IsStatic;
  double scale = 1.f;
  int Spaces = 0;
  bool read = false; // If the text was already read by a NPC
};

} // namespace components
} // namespace stella
