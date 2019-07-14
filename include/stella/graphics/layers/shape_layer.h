#pragma once

#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "stella/graphics/shader.h"
#include "stella/graphics/sprite.h"
#include "stella/graphics/shape_renderer.h"

namespace stella {
namespace graphics {
class ShapeLayer {
protected:
  std::shared_ptr<ShapeRenderer> Ren;
  std::shared_ptr<Shader> Shad;
  std::vector<std::shared_ptr<Sprite>> Sprites;
  glm::mat4 ViewMatrix;

public:
  bool Fixed;

  virtual ~ShapeLayer();
  virtual void Add(std::shared_ptr<Sprite> sprite);
  virtual void Remove(std::shared_ptr<Sprite> sprite);
  virtual void Render();
  virtual void SetViewMatrix(glm::mat4 view);

protected:
  ShapeLayer(std::shared_ptr<ShapeRenderer> renderer, bool fixed = true);
};
} // namespace graphics
} // namespace stella

