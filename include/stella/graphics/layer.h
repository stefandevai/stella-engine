#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "stella/graphics/shader.h"
#include "stella/graphics/sprite.h"
#include "stella/graphics/renderer.h"

namespace stella {
namespace graphics {
class Layer {
public:
  bool Fixed;

  virtual ~Layer();
  virtual void Add(std::shared_ptr<Sprite> sprite);
  virtual void Remove(std::shared_ptr<Sprite> sprite);
  virtual void Render();
  virtual void SetViewMatrix(glm::mat4 view);

protected:
  std::shared_ptr<Renderer> Ren;
  std::shared_ptr<Shader> Shad;
  std::vector<std::shared_ptr<Sprite>> Sprites;
  glm::mat4 ViewMatrix;

  Layer(std::shared_ptr<Renderer> renderer, bool fixed = true);
};
} // namespace graphics
} // namespace stella
