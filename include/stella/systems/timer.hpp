#pragma once

#include "stella/systems/system.hpp"

namespace stella
{
namespace system
{
  class Timer : public System
  {
  public:
    Timer (entt::registry& registry);
    void update (entt::registry& registry, const double dt);

  private:
    Timer() = delete;
  };
} // namespace system
} // namespace stella
