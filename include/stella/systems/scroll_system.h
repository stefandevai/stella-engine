#pragma once

#include <glm/glm.hpp>
#include <entityx/entityx.h>
namespace ex = entityx;

namespace stella {
namespace systems {
class ScrollSystem : public ex::System<ScrollSystem> {
public:
  ScrollSystem();
  ~ScrollSystem();
  void update(ex::EntityManager &es, ex::EventManager &events, ex::TimeDelta dt) override;

private:
  glm::vec2 GlobalScrollingSpeed = glm::vec2(0.f, 0.f);
};
} // namespace systems
} // namespace stella

