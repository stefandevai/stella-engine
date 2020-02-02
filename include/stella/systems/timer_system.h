#pragma once

#include "stella/components/timer.h"
#include "stella/systems/system.h"

namespace stella
{
namespace systems
{
  class TimerSystem : public System
  {
  public:
    TimerSystem (entt::registry& registry)
    {
      //   registry.on_construct<components::Timer>().connect<&TimerSystem::initialize_text>(this);
    }

    ~TimerSystem() override {}

    void update (entt::registry& registry, const double dt) override
    {
      registry.view<components::Timer>().each ([&registry, dt] (auto entity, auto& timer) {
        timer.duration -= (int) (dt * 1000);

        if (timer.duration <= 0)
        {
          switch (timer.event)
          {
            case components::Timer::TimerEvent::Destroy:
              registry.destroy (entity);
              break;
            default:
              break;
          }
        }
      });
    }

  private:
    TimerSystem() = delete;

    // void initialize_text(entt::registry &registry, entt::entity entity,
    // components::Timer &timer)
    // {
    //   //auto pos = registry.get<components::Position>(entity);

    // }
  };
} // namespace systems
} // namespace stella
