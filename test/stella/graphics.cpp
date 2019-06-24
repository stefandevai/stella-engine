#include <stella/graphics.h>

#include "../catch.hpp"

namespace {
  // Display required to load OpenGL context
  stella::graphics::Display display(720, 405, "T O R C H");

  void test_sprite_initialization(const stella::graphics::Sprite &sprite) {
    REQUIRE(sprite.GetCurrentFrame() == 0);
    REQUIRE(sprite.GetColor() == 4294967295);
  }
}

TEST_CASE("textures can be loaded and assigned to sprites", "[lib][graphics]")
{
  stella::graphics::Texture texture("test-texture", "./assets/texture.png");
  stella::graphics::Sprite sprite1(0, 0, texture);

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
  }

  SECTION("testingo")
  {
    REQUIRE(1 == 1);
  }
}

