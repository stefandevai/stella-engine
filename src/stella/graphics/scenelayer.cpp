#include "stella/graphics/scenelayer.h"

namespace stella {
namespace graphics {
SceneLayer::SceneLayer(stella::graphics::Shader *shader,
                       const glm::mat4 &projection)
    : Layer(new stella::graphics::Renderer(), shader, projection) {}

SceneLayer::~SceneLayer() {}
} // namespace graphics
} // namespace stella
