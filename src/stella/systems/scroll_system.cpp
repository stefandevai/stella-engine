#include "stella/components.h"
#include "stella/systems/scroll_system.h"

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
      pos.x += final_speed.x*dt;
      pos.y += final_speed.y*dt;
  });
}
} // namespace systems
} // namespace stella

