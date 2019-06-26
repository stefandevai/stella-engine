#include "stella/graphics/scenelayer.h"

#include <memory>

namespace stella {
namespace graphics {
SceneLayer::SceneLayer(stella::graphics::Shader *shader,
                       const glm::mat4 &projection)
    : Layer(std::unique_ptr<Renderer>(new Renderer())) {}

SceneLayer::~SceneLayer() {}
} // namespace graphics
} // namespace stella
