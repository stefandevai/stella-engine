#pragma once

#include "./layer.h"
#include "../shader.h"

namespace stella {
namespace graphics {
class BasicLayer : public Layer {
public:
  BasicLayer(float width, float height, const char *vspath, const char *fspath, bool fixed = true);
  ~BasicLayer();
};
} // namespace graphics
} // namespace stella

