#pragma once

#include <stella/graphics/layer.h>

namespace stella { namespace graphics {
  class SceneLayer : public Layer
  {
    public:
      SceneLayer(Shader* shader, const glm::mat4 &projection);
      ~SceneLayer();
  };
} }

