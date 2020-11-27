#pragma once

#include "component.hpp"
#include "stella/topdown/body.hpp"
#include <vector>
#include <memory>

namespace stella
{
namespace component
{
  struct Body2D : public Component
  {
    Body2D (const std::vector<double>& bounding_box = {0.f, 0.f}, const std::vector<double>& bounding_box_position = {0.f, 0.f}, const float movement_speed = 20.f)
      : Component ("Body 2D"), BoundingBox (bounding_box), BoundingBoxPosition (bounding_box_position), movement_speed (movement_speed)
    {
    }

    std::shared_ptr<stella::topdown::Body> Body = nullptr;
    bool Initialized                            = false;
    std::vector<double> BoundingBox, BoundingBoxPosition;
    float movement_speed = 0.f;
  };
} // namespace component
} // namespace stella
