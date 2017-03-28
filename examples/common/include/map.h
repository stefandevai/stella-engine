#pragma once

#include <vector>

#include <stella/graphics/texture.h>
#include <stella/graphics/sprite.h>

namespace stella { namespace graphics {
  class Map
  {
    public:
      Map(const char* filename, stella::graphics::Texture *texture);
      ~Map();

    private:
      stella::graphics::Texture *Tex;
      std::vector<std::vector<Sprite*>> Layers;
      void load(const char* filename);
  };
} }

