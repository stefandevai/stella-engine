#pragma once

#include <string>
#include <vector>

#include <entt/entity/registry.hpp>

namespace stella
{
namespace component
{
  struct Timer
  {
    enum TimerEvent
    {
      Destroy,
      Pause
    };

    Timer (TimerEvent event, const int duration) : event (event), duration (duration) {}

    TimerEvent event;
    int duration;
  };

} // namespace component
} // namespace stella
