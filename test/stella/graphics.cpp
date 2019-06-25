#include <iostream>
#include <stella/graphics.h>

#include "../catch.hpp"

namespace {
  // Display required to load OpenGL context
  stella::graphics::Display display(0, 0, "");

  stella::graphics::Texture texture("test-texture", "./assets/texture.png");
  stella::graphics::Sprite sprite1(0, 0, texture);
  stella::graphics::Sprite sprite2(0, 0, 10, 10, texture);
  std::vector<GLuint> frames{0, 1, 2, 3, 4};

  stella::graphics::Sprite sprite3(0, 0, texture, frames);

  void test_sprite_initialization(const stella::graphics::Sprite &sprite) {
    REQUIRE(sprite.GetCurrentFrame() == 0);
    REQUIRE(sprite.GetColor() == 4294967295);
  }
}

TEST_CASE("textures can be loaded and assigned to sprites", "[lib][graphics]")
{
  SECTION("texture is loaded properly")
  {
    REQUIRE(texture.GetName() == "test-texture");
    REQUIRE(texture.GetWidth() == 255);
    REQUIRE(texture.GetHeight() == 170);
    REQUIRE(texture.IsCached() == false);
  }

  SECTION("sprites are loaded properly")
  {
    test_sprite_initialization(sprite1);
    REQUIRE(sprite1.GetWidth() == 255);
    REQUIRE(sprite1.GetHeight() == 170);

    test_sprite_initialization(sprite2);
    REQUIRE(sprite2.GetWidth() == 10);
    REQUIRE(sprite2.GetHeight() == 10);

    test_sprite_initialization(sprite3);
    REQUIRE(sprite3.GetWidth() == 255);
    REQUIRE(sprite3.GetHeight() == 170);
  }
}

TEST_CASE("sprites can have frames and animations", "[lib][graphics]")
{
  sprite3.Animations.Add("first-animation", std::vector<unsigned int>{0,1,2,3,4}, 1);
  sprite3.Animations.Add("second-animation", std::vector<unsigned int>{3,1,0,3,4,6}, 1);

  SECTION("frames can be set and updated")
  {
    // Set frame as modulus of number of frames
    sprite3.SetFrame(12);
    REQUIRE(sprite3.GetCurrentFrame() == 2);

    // Directtly set a frame
    sprite3.SetDirectFrame(4);
    REQUIRE(sprite3.GetCurrentFrame() == 4);
  }

  SECTION("animations can be played, paused and reseted")
  {
    // If we are in the last frame, the next will be the first
    sprite3.Animations.Play("first-animation");
    sprite3.Update();
    REQUIRE(sprite3.GetCurrentFrame() == 0);
    sprite3.Update();
    sprite3.Update();
    sprite3.Update();
    sprite3.Update();
    REQUIRE(sprite3.GetCurrentFrame() == 4);
    sprite3.Update();
    REQUIRE(sprite3.GetCurrentFrame() == 0);
    sprite3.Update();
    sprite3.Update();
    sprite3.Animations.Pause();
    sprite3.Update();
    sprite3.Animations.TogglePause();
    sprite3.Update();
    REQUIRE(sprite3.GetCurrentFrame() == 3);
    sprite3.Animations.Reset();
    sprite3.Update();
    REQUIRE(sprite3.GetCurrentFrame() == 0);
  }

  SECTION("animation frames can have any order")
  {
    sprite3.Animations.Play("second-animation");
    sprite3.Update();
    REQUIRE(sprite3.GetCurrentFrame() == 3);
    sprite3.Update();
    REQUIRE(sprite3.GetCurrentFrame() == 1);
    sprite3.Animations.TogglePause();
    sprite3.Update();
    sprite3.Animations.TogglePause();
    sprite3.Update();
    REQUIRE(sprite3.GetCurrentFrame() == 0);
    sprite3.Animations.Reset();
    sprite3.Update();
    REQUIRE(sprite3.GetCurrentFrame() == 3);
  }
}

