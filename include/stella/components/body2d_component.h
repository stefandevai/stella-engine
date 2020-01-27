#pragma once

#include <stella/topdown/body.h>
#include <vector>

namespace stella
{
namespace components
{
  struct Body2DComponent
  {
    Body2DComponent (const std::vector<double>& drag                  = {0.f, 0.f},
                     const std::vector<double>& bounding_box          = {0.f, 0.f},
                     const std::vector<double>& bounding_box_position = {0.f, 0.f},
                     bool collide_with_borders                        = false)
      : Drag (drag), BoundingBox (bounding_box), BoundingBoxPosition (bounding_box_position),
        CollideWithBorders (collide_with_borders)
    {
    }
    ~Body2DComponent() {}

    std::shared_ptr<stella::topdown::Body> Body;
    bool Initialized = false;
    std::vector<double> Drag, BoundingBox, BoundingBoxPosition;
    bool CollideWithBorders;
  };
} // namespace components
} // namespace stella
