#pragma once

#include <string>

#include <stella/stella.h>

struct SpriteComponent {
  inline SpriteComponent(std::string tex_name) : TexName(tex_name) {
    InLayer = false;
    Initialized = false;
  }

	stella::graphics::Sprite *Sprite;
	std::string TexName;
  bool InLayer, Initialized;
};

