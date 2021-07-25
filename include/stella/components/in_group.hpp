#pragma once

#include "component.hpp"

namespace stella
{
namespace component
{
  struct InGroup : public Component
  {
    InGroup() : Component ("In Group") {}
  };
} // namespace component
} // namespace stella
