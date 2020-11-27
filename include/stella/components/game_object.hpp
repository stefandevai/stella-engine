#pragma once

#include "component.hpp"

namespace stella
{
namespace component
{
  struct GameObject : public Component
  {
    GameObject() : Component ("GameObject") {}
  };
} // namespace component
} // namespace stella
