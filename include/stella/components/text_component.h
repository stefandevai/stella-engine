#pragma once

#include <string>
#include <vector>

#include <entt/entity/registry.hpp>

namespace stella {
namespace components {

struct TextComponent {
  TextComponent(const std::string text, const std::string font_name, const std::string color = "#ffffffff", bool is_static = true)
    : Text(text), FontName(font_name), color(color), IsStatic(is_static)
  {
    
  }
  std::string Text, FontName, color;
  std::vector<entt::registry::entity_type> char_entities;
  bool InLayer = false, IsStatic;
  int Spaces = 0;
};

} // namespace components
} // namespace stella
