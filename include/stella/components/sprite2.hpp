#pragma once

#include <string>
#include <memory>
#include <glm/vec2.hpp>
namespace stella{ namespace graphics{ class Texture; } }

namespace stella
{
namespace component
{
  struct SpriteT
  {
      SpriteT (const std::string& texture) : texture (texture) {}
      
      std::string texture = "";
      std::shared_ptr<graphics::Texture> texture_ptr = nullptr;
      std::string layer = "";
      int frame = 0;
      unsigned int hframes = 1;
      unsigned int vframes = 1;
      bool loaded = false;

      int width();
      int height();
      glm::vec2 uv();
      void set_texture(const std::string& texture);
  };
}
}