#pragma once

#include "component.hpp"

namespace stella
{
namespace component
{
  struct Camera : public Component
  {
    Camera() : Component ("Camera") {}
    uint8_t id = 0;
  };
} // namespace component
} // namespace stella
