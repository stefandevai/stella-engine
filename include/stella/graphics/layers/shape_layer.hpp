#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <vector>

#include "stella/graphics/shader.hpp"
#include "stella/graphics/shape.hpp"
#include "stella/graphics/shape_renderer.hpp"

namespace stella
{
namespace graphics
{
  class ShapeLayer
  {
  protected:
    std::shared_ptr<ShapeRenderer> Ren;
    std::shared_ptr<Shader> Shad;
    std::vector<std::shared_ptr<Shape>> Shapes;
    glm::mat4 ViewMatrix;

  public:
    bool Fixed;

    virtual ~ShapeLayer();
    virtual void Add (std::shared_ptr<Shape> shape);
    virtual void Remove (std::shared_ptr<Shape> shape);
    virtual void Render();
    virtual void SetViewMatrix (glm::mat4 view);

  protected:
    ShapeLayer (std::shared_ptr<ShapeRenderer> renderer, bool fixed = true);
  };
} // namespace graphics
} // namespace stella
