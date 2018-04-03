#pragma once

#include <bitset>
#include <memory>
#include <unordered_set>
#include <utility>
#include <vector>

#include <entityx/entityx.h>
#include <stella/stella.h>

#include "../components/game_components.h"

namespace ex = entityx;

class CollisionSystem : public ex::System<CollisionSystem> {
public:
  CollisionSystem(int w, int h);
  ~CollisionSystem();
  void update(ex::EntityManager &es, ex::EventManager &events,
              ex::TimeDelta dt) override;

private:
  bool check_collision(ex::Entity &b1, ex::Entity &b2);
};
