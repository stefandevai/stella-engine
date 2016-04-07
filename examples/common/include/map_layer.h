#pragma once

#include <vector>

namespace stella { namespace graphics {
  class MapLayer
  {
    public:
      virtual ~MapLayer() = 0;
      virtual void Update() = 0;

    protected:
      std::vector<int> &Data;
      unsigned int Width, Height;
      int OffsetX, OffsetY;

      MapLayer(std::vector<int> &data, unsigned int width, unsigned int height, int offsetx = 0, int offsety = 0);
  };
} }

