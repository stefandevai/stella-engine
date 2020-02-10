#pragma once

#include <stella/topdown/body.h>
#include <vector>

namespace stella
{
namespace component
{
  struct Body2D
  {
    Body2D (const std::vector<double>& bounding_box          = {0.f, 0.f},
            const std::vector<double>& bounding_box_position = {0.f, 0.f},
            const float movement_speed = 20.f)
      : BoundingBox (bounding_box), BoundingBoxPosition (bounding_box_position),
        movement_speed (movement_speed)
    {
    }

    std::shared_ptr<stella::topdown::Body> Body;
    bool Initialized = false;
    std::vector<double> BoundingBox, BoundingBoxPosition;
    float movement_speed;
  };
} // namespace component
} // namespace stella
