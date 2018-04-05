#pragma once

#include <string>

#include <glm/glm.hpp>
#include <stella/stella.h>

struct SpriteComponent {
  inline SpriteComponent(std::string tex_name) : TexName(tex_name) { }
  inline SpriteComponent(std::string tex_name, glm::vec2 frame_dimensions, unsigned int frame = 0) : TexName(tex_name), FrameDimensions(frame_dimensions), Frame(frame) { }

	stella::graphics::Sprite *Sprite;
	std::string TexName;
  glm::vec2 FrameDimensions = glm::vec2(0.f, 0.f);
  unsigned int Frame = 0;
  bool InLayer = false, Initialized = false, HasScaled = false;
};

