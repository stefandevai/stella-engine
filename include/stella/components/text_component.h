#pragma once

#include <string>
#include <vector>

#include <stella/stella.h>

namespace stella {
namespace components {
struct TextComponent {
  TextComponent(const std::string &text, const std::string &font_tex_name, bool is_static = false) : Text(text), Name(font_tex_name), IsStatic(is_static) { this->InLayer = false; }
	std::string Text, Name;
	std::vector<std::shared_ptr<stella::graphics::Sprite>> Sprites;
	bool InLayer, IsStatic;
  int Spaces = 0;
};
} // namespace components
} // namespace stella

