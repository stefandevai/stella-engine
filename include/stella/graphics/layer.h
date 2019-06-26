#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "stella/graphics/shader.h"
#include "stella/graphics/sprite.h"
#include "stella/graphics/renderer.h"

namespace stella {
namespace graphics {

class Layer {
public:
  virtual ~Layer();
  virtual void Add(std::shared_ptr<Sprite> sprite);
  virtual void Remove(std::shared_ptr<Sprite> sprite);
  virtual void Render();

protected:
  std::shared_ptr<Renderer> Ren;
  std::shared_ptr<Shader> Shad;
  std::vector<std::shared_ptr<Sprite>> Sprites;

  Layer(std::shared_ptr<Renderer> renderer);
};
} // namespace graphics
} // namespace stella
