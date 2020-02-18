#pragma once

#include "stella/systems/system.h"

namespace stella
{
namespace system
{
  class Speech : public System
  {
  public:
    Speech();
    void update (entt::registry& registry, const double dt) override;
  };
} // namespace system
} // namespace stella
