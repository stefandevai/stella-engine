#pragma once

#include "layer.h"
#include "shader.h"

namespace stella {
namespace graphics {
class SceneLayer : public Layer {
public:
  SceneLayer(stella::graphics::Shader *shader, const glm::mat4 &projection);
  ~SceneLayer();
};
} // namespace graphics
} // namespace stella
