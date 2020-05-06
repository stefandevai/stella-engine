#pragma once

#include "component.hpp"

namespace stella
{
namespace component
{
  struct CharacterAnimation : public Component
  {
    CharacterAnimation() : Component ("Character Animation") {}
    int place_holder = 0;
  };
} // namespace component
} // namespace stella
