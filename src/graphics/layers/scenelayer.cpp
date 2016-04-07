#include "graphics/scenelayer.h"

namespace stella { namespace graphics {
  SceneLayer::SceneLayer(Shader* shader, const glm::mat4 &projection)
    : Layer(new Renderer(), shader, projection)
  {

  }

  SceneLayer::~SceneLayer()
  {

  }
} }

