#pragma once

#include <stella/graphics/sprite.h>
#include <stella/graphics/texture.h>
#include <anax/Component.hpp>
#include <iostream>

struct SpriteComponent : public anax::Component
{
  public:
    inline SpriteComponent(int w, int h, stella::graphics::Texture &texture, int frame = 0) { sprite = new stella::graphics::Sprite(0, 0, w, h, texture, frame); }
    stella::graphics::Sprite *sprite;
};

