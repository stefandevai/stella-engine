#include "stella/components.h"
#include "stella/systems/scroll_system.h"

#include <math.h>

namespace stella {
namespace systems {
ScrollSystem::ScrollSystem() {
}

ScrollSystem::~ScrollSystem() {

}

void ScrollSystem::update(ex::EntityManager &es, ex::EventManager &events, ex::TimeDelta dt) {
  es.each<components::ScrollComponent, components::PositionComponent>([this, &dt](ex::Entity entity,
                                                      components::ScrollComponent &scroll,
                                                      components::PositionComponent &pos) {
      glm::vec2 final_speed = scroll.speed + this->GlobalScrollingSpeed;
      pos.x += ceil(final_speed.x*dt);
      pos.y += ceil(final_speed.y*dt);

      if (entity.has_component<components::Body2DComponent>())
      {
        auto body = entity.component<components::Body2DComponent>();
        body->Body->Position.x += ceil(final_speed.x*dt);
        body->Body->Position.y += ceil(final_speed.y*dt);
      }
  });
}
} // namespace systems
} // namespace stella

