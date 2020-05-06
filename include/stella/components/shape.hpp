#pragma once

#include "component.hpp"

#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <memory>
//#include "stella/graphics/shape.hpp"

namespace stella
{
namespace component
{
  struct Shape : public Component
  {
    Shape() : Component ("Shape") {}
    Shape (std::vector<glm::vec3>& vertices, std::string layer_id)
      : Component ("Shape"), vertices (vertices), layer_id (layer_id)
    {
      // shape = std::make_shared<graphics::Shape>(vertices, glm::vec3());
    }

    inline glm::vec3 calc_dimensions()
    {
      glm::vec3 min{0.f};
      glm::vec3 max{0.f};

      for (const auto& vertex : vertices)
      {
        if (vertex.x > max.x)
        {
          max.x = vertex.x;
        }
        else if (vertex.x < min.x)
        {
          min.x = vertex.x;
        }

        if (vertex.y > max.y)
        {
          max.y = vertex.y;
        }
        else if (vertex.y < min.y)
        {
          min.y = vertex.y;
        }

        if (vertex.z > max.z)
        {
          max.z = vertex.z;
        }
        else if (vertex.z < min.z)
        {
          min.z = vertex.z;
        }
      }

      return max - min;
    }

    std::vector<glm::vec3> vertices{};
    bool in_layer        = false;
    std::string layer_id = "";
    // std::shared_ptr<graphics::Shape> shape = nullptr;
  };
} // namespace component
} // namespace stella
