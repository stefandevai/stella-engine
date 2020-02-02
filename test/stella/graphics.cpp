#include "stella/graphics.h"
#include "stella/components/color.h"
#include <iostream>

#include "../catch.hpp"

namespace
{
// Display CHECKd to load OpenGL context
stella::graphics::Display display (0, 0, "");

stella::graphics::Texture texture ("assets/texture.png");
stella::graphics::Sprite sprite1 (0, 0, texture);
stella::graphics::Sprite sprite2 (0, 0, 10, 10, texture);
std::vector<GLuint> frames{0, 1, 2, 3, 4};

stella::graphics::Sprite sprite3 (0, 0, texture, frames);

void test_sprite_initialization (const stella::graphics::Sprite& sprite)
{
  CHECK (sprite.GetCurrentFrame() == 0);
  CHECK (sprite.GetColor() == 4294967295);
}
} // namespace

TEST_CASE ("textures can be loaded and assigned to sprites", "[lib][graphics]")
{
  SECTION ("texture is loaded properly")
  {
    CHECK (texture.GetWidth() == 255);
    CHECK (texture.GetHeight() == 170);
    CHECK (texture.IsCached() == false);
  }

  SECTION ("sprites are loaded properly")
  {
    test_sprite_initialization (sprite1);
    CHECK (sprite1.GetWidth() == 255);
    CHECK (sprite1.GetHeight() == 170);

    test_sprite_initialization (sprite2);
    CHECK (sprite2.GetWidth() == 10);
    CHECK (sprite2.GetHeight() == 10);

    test_sprite_initialization (sprite3);
    CHECK (sprite3.GetWidth() == 255);
    CHECK (sprite3.GetHeight() == 170);
  }
}

TEST_CASE ("sprites can have frames and animations", "[lib][graphics]")
{
  sprite3.Animations.Add ("first-animation", std::vector<unsigned int>{0, 1, 2, 3, 4}, 1);
  sprite3.Animations.Add ("second-animation", std::vector<unsigned int>{3, 1, 0, 3, 4, 6}, 1);

  SECTION ("frames can be set and updated")
  {
    // Set frame as modulus of number of frames
    sprite3.SetFrame (12);
    CHECK (sprite3.GetCurrentFrame() == 2);

    // Directtly set a frame
    sprite3.SetDirectFrame (4);
    CHECK (sprite3.GetCurrentFrame() == 4);
  }

  SECTION ("animations can be played, paused and reseted")
  {
    // If we are in the last frame, the next will be the first
    sprite3.Animations.Play ("first-animation");
    sprite3.Update();
    CHECK (sprite3.GetCurrentFrame() == 0);
    sprite3.Update();
    sprite3.Update();
    sprite3.Update();
    sprite3.Update();
    CHECK (sprite3.GetCurrentFrame() == 4);
    sprite3.Update();
    CHECK (sprite3.GetCurrentFrame() == 0);
    sprite3.Update();
    sprite3.Update();
    sprite3.Animations.Pause();
    sprite3.Update();
    sprite3.Animations.TogglePause();
    sprite3.Update();
    CHECK (sprite3.GetCurrentFrame() == 3);
    sprite3.Animations.Reset();
    sprite3.Update();
    CHECK (sprite3.GetCurrentFrame() == 0);
  }

  SECTION ("animation frames can have any order")
  {
    sprite3.Animations.Play ("second-animation");
    sprite3.Update();
    CHECK (sprite3.GetCurrentFrame() == 3);
    sprite3.Update();
    CHECK (sprite3.GetCurrentFrame() == 1);
    sprite3.Animations.TogglePause();
    sprite3.Update();
    sprite3.Animations.TogglePause();
    sprite3.Update();
    CHECK (sprite3.GetCurrentFrame() == 0);
    sprite3.Animations.Reset();
    sprite3.Update();
    CHECK (sprite3.GetCurrentFrame() == 3);
  }
}

TEST_CASE ("Color component conversions")
{
  stella::components::Color color = stella::components::Color ("#4db88fff");
  CHECK (color.hex_color == "#4db88fff");
  CHECK (color.int_color == 1303941119);
  CHECK (color.rgba_color.r == 77);
  CHECK (color.rgba_color.g == 184);
  CHECK (color.rgba_color.b == 143);
  CHECK (color.rgba_color.a == 255);

  stella::components::Color color2 = stella::components::Color (77, 184, 143, 255);
  CHECK (color2.hex_color == "#4db88fff");
  CHECK (color2.int_color == 1303941119);
  CHECK (color2.rgba_color.r == 77);
  CHECK (color2.rgba_color.g == 184);
  CHECK (color2.rgba_color.b == 143);
  CHECK (color2.rgba_color.a == 255);
}
