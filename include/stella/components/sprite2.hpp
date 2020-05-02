#pragma once

#include <string>
namespace stella{ namespace graphics{ class Texture; } }

namespace stella
{
namespace component
{
  struct SpriteT
  {
      SpriteT (const std::string& texture) : texture (texture) {}
      int frame = 0;
      unsigned int hframe = 1;
      unsigned int vframes = 1;
      std::string texture = "";
      std::string layer = "";
      std::shared_ptr<graphics::Texture> texture_ptr = nullptr;
      bool loaded = false;
  };
}
}