#pragma once

#include "../components.h"
#include "./system.h"

#include <glm/glm.hpp>

namespace stella
{
namespace systems
{
  class ScrollSystem : public System
  {
  private:
    glm::vec2 GlobalScrollingSpeed = glm::vec2 (0.f, 0.f);

  public:
    ScrollSystem() {}

    ~ScrollSystem() override {}

    void update (entt::registry& registry, const double dt) override
    {
      registry.group<components::Scroll> (entt::get<components::Position>)
          .each ([this, dt] (auto entity, auto& scroll, auto& pos) {
            auto final_speed = scroll.speed + this->GlobalScrollingSpeed;
            pos.x += final_speed.x * dt;
            pos.y += final_speed.y * dt;
          });
    }
  };
} // namespace systems
} // namespace stella
