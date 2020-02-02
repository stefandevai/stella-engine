#pragma once

#include "stella/components/timer.h"
#include "stella/systems/system.h"

namespace stella
{
namespace systems
{
  class Timer : public System
  {
  public:
    Timer (entt::registry& registry)
    {
      //   registry.on_construct<component::Timer>().connect<&Timer::initialize_text>(this);
    }

    ~Timer() override {}

    void update (entt::registry& registry, const double dt) override
    {
      registry.view<component::Timer>().each ([&registry, dt] (auto entity, auto& timer) {
        timer.duration -= (int) (dt * 1000);

        if (timer.duration <= 0)
        {
          switch (timer.event)
          {
            case component::Timer::TimerEvent::Destroy:
              registry.destroy (entity);
              break;
            default:
              break;
          }
        }
      });
    }

  private:
    Timer() = delete;

    // void initialize_text(entt::registry &registry, entt::entity entity,
    // component::Timer &timer)
    // {
    //   //auto pos = registry.get<component::Position>(entity);

    // }
  };
} // namespace systems
} // namespace stella