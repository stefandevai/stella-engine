#include "stella/graphics/layers/basic_layer.h"

namespace stella {
namespace graphics {
BasicLayer::BasicLayer(stella::graphics::Shader *shader,
                       const glm::mat4 &projection)
  : Layer(std::shared_ptr<Renderer>(new Renderer), shader, projection) {
      //new stella::graphics::Renderer()
  }
BasicLayer::~BasicLayer() {}
} // namespace graphics
} // namespace stella

