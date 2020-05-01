#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <memory>
#include "stella/graphics/shape.hpp"

namespace stella
{
namespace component
{
  struct Shape
  {
    Shape (std::vector<glm::vec2>& vertices, std::string layer_id) : vertices (vertices), layer_id(layer_id)
    {
      shape = std::make_shared<graphics::Shape>(vertices, glm::vec3());
    }

    std::vector<glm::vec2> vertices;
    bool in_layer;
    std::string layer_id;
    std::shared_ptr<graphics::Shape> shape;
  };
} // namespace component
} // namespace stella
