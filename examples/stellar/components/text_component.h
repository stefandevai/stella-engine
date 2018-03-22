#pragma once

#include <string>
#include <vector>

#include <stella/stella.h>

struct TextComponent {
  TextComponent(std::string text, std::string font_tex_name, bool is_static = false) : Text(text), Name(font_tex_name), IsStatic(is_static) { this->InLayer = false; }
	std::string Text, Name;
	std::vector<stella::graphics::Sprite*> Sprites;
	bool InLayer, IsStatic;
	int Spaces;
};

