#pragma once

#include <stella/graphics/layer.h>
#include <stella/graphics/shader.h>

class SceneLayer : public stella::graphics::Layer {
public:
  SceneLayer(stella::graphics::Shader *shader, const glm::mat4 &projection);
  ~SceneLayer();
};
