#pragma once

#include <string>

#include "../graphics/sprite.h"
#include <glm/glm.hpp>

namespace stella
{
namespace component
{
  struct Sprite
  {
    Sprite (const std::string& tex_name, glm::vec2 frame_dimensions, std::string layer_id = "basic", unsigned frame = 0)
      : TexName (tex_name), FrameDimensions (frame_dimensions), LayerId (layer_id), Frame (frame)
    {
    }
    Sprite (const std::string& tex_name,
            const float framex,
            const float framey,
            std::string layer_id = "basic",
            unsigned frame       = 0)
      : TexName (tex_name), FrameDimensions (glm::vec2 (framex, framey)), LayerId (layer_id), Frame (frame)
    {
    }
    Sprite (const std::string& tex_name, std::string layer_id = "basic") : TexName (tex_name), LayerId (layer_id) {}
    Sprite (const glm::vec3 position,
            const glm::vec2 dimensions,
            const glm::vec2 offset,
            graphics::Texture& texture,
            std::string layer_id = "basic")
      : LayerId (layer_id)
    {
      this->sprite = std::shared_ptr<graphics::Sprite> (new graphics::Sprite (position, dimensions, offset, texture));
      this->Initialized = true;
    }
    Sprite (
        graphics::Texture& texture, const float framex, const float framey, int frame, std::string layer_id = "basic")
      : FrameDimensions (glm::vec2 (framex, framey)), LayerId (layer_id), Frame (frame)
    {
      this->sprite = std::shared_ptr<graphics::Sprite> (new graphics::Sprite (0, 0, framex, framey, texture, frame));
      this->Initialized = true;
    }

    std::shared_ptr<stella::graphics::Sprite> sprite = nullptr;
    std::string TexName;
    glm::vec2 FrameDimensions = glm::vec2 (0.f, 0.f);
    std::string LayerId;
    unsigned int Frame = 0;
    bool InLayer = false, Initialized = false, HasScaled = false;
  };
} // namespace component
} // namespace stella
