#pragma once

#include <string>
#include <vector>

#include <entt/entity/registry.hpp>

namespace stella {
namespace components {

struct TimerComponent {
  enum TimerEvent { Destroy, Pause };
  
  TimerComponent(TimerEvent event, const int duration)
    : event(event), duration(duration)
  {}

  TimerEvent event;
  int duration;
};

} // namespace components
} // namespace stella
