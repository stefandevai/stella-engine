#pragma once

#include <string>

struct TextComponent {
  TextComponent(std::string text, std::string font_tex_name) : Text(text), Name(font_tex_name) { this->InLayer = false; }
	std::string Text, Name;
	bool InLayer;
};

