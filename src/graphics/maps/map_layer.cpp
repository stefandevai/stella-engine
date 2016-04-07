#include "graphics/map_layer.h"

namespace stella { namespace graphics {
  MapLayer::MapLayer(std::vector<int> &data, unsigned int width, unsigned int height, int offsetx, int offsety)
    : Data(data), Width(width), Height(height), OffsetX(offsetx), OffsetY(offsety)
  {

  }
} }

