#pragma once

#include "../components.hpp"
#include "./system.hpp"

#include <glm/glm.hpp>

namespace stella
{
namespace system
{
  class Scroll : public System
  {
  private:
    glm::vec2 GlobalScrollingSpeed = glm::vec2 (0.f, 0.f);

  public:
    Scroll() {}

    ~Scroll() override {}

    void update (entt::registry& registry, const double dt) override
    {
      registry.group<component::Scroll> (entt::get<component::Position>)
          .each ([this, dt] (auto entity, auto& scroll, auto& pos) {
            auto final_speed = scroll.speed + this->GlobalScrollingSpeed;
            pos.x += final_speed.x * dt;
            pos.y += final_speed.y * dt;
          });
    }
  };
} // namespace system
} // namespace stella
