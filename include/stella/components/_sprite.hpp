#pragma once

#include "component.hpp"

#include <string>
#include <memory>
#include <glm/vec2.hpp>
namespace stella
{
namespace graphics
{
  class Texture;
}
} // namespace stella

namespace stella
{
namespace component
{
  struct SpriteT : public Component
  {
    SpriteT() : Component ("Sprite") {}
    SpriteT (const std::string& texture) : Component ("Sprite"), texture (texture) {}

    std::string texture                            = "";
    std::shared_ptr<graphics::Texture> texture_ptr = nullptr;
    std::string layer                              = "";
    int frame                                      = 0;
    // Coords to form an area for the sprite in the texture
    glm::vec2 top_left     = glm::vec2{0.0f, 0.0f};
    glm::vec2 bottom_right = glm::vec2{0.0f, 0.0f};
    glm::vec2 uv{-1.0, -1.0};
    bool user_has_set_uv = false;

    int get_width();
    int get_height();
    glm::vec2 get_uv();
    void set_texture (const std::string& texture);
    void set_uv (const glm::vec2& uv);
  };
} // namespace component
} // namespace stella
