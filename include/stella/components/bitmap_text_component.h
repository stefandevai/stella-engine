#pragma once

#include <string>
#include <vector>

#include <entt/entity/registry.hpp>

namespace stella {
namespace components {
struct BitmapTextComponent {
  BitmapTextComponent(const std::string &text, const std::string &font_tex_name, bool is_static = false) : Text(text), Name(font_tex_name), IsStatic(is_static) { this->InLayer = false; }
	std::string Text, Name;
	std::vector<entt::registry::entity_type> char_entities;
	bool InLayer, IsStatic;
  int Spaces = 0;
};
} // namespace components
} // namespace stella

