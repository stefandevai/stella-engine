#pragma once

#include <stella/stella.h>

struct TextureComponent
{
	inline TextureComponent(int w, int h, stella::graphics::Texture &texture, int frame = 0) { sprite = new stella::graphics::Sprite(0, 0, w, h, texture, frame); InLayer = false; }
	stella::graphics::Sprite *sprite;
	bool InLayer;
};

