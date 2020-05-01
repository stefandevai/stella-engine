#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <set>
#include <vector>

#include "stella/graphics/sprite_renderer.hpp"
#include "stella/graphics/shader.hpp"
#include "stella/graphics/sprite.hpp"

namespace stella
{
namespace graphics
{
  class Layer
  {
  protected:
    std::shared_ptr<SpriteRenderer> Ren;
    std::shared_ptr<Shader> Shad;
    glm::mat4 ViewMatrix;

  public:
    bool Fixed;

    virtual ~Layer();
    virtual void Add (std::shared_ptr<Sprite> sprite) = 0;
    virtual void Remove (std::shared_ptr<Sprite> sprite) = 0;
    virtual void Render() = 0;
    virtual void SetViewMatrix (glm::mat4 view);

  protected:
    Layer (std::shared_ptr<SpriteRenderer> renderer, bool fixed = true);
  };
} // namespace graphics
} // namespace stella
