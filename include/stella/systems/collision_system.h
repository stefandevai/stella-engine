#pragma once

#include <entityx/entityx.h>

namespace ex = entityx;

namespace stella {
namespace systems {
class CollisionSystem : public ex::System<CollisionSystem> {
public:
  CollisionSystem(int w, int h);
  ~CollisionSystem();
  void update(ex::EntityManager &es, ex::EventManager &events,
              ex::TimeDelta dt) override;

private:
  bool check_collision(ex::Entity &b1, ex::Entity &b2);
};
} // namespace systems
} // namespace stella

