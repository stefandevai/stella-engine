#pragma once

#include <vector>

#include "map_layer.h"
#include "../spritesheet.h"

namespace stella { namespace graphics {
  class Map
  {
    public:
      virtual ~Map();
      virtual void Update();

    protected:
      std::vector<MapLayer*> Layers;
      SpriteSheet Frames;

      Map(const char* filename, const SpriteSheet &spritesheet);
      void loadData(const char *filename);
  };
} }

