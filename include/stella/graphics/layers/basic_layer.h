#pragma once

#include "../layer.h"
#include "../shader.h"

namespace stella {
namespace graphics {
class BasicLayer : public Layer {
public:
  BasicLayer(stella::graphics::Shader *shader, const glm::mat4 &projection);
  ~BasicLayer();
};
} // namespace graphics
} // namespace stella

