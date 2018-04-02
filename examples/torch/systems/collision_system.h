#pragma once

#include <bitset>
#include <memory>
#include <unordered_set>
#include <utility>
#include <vector>

#include <entityx/entityx.h>
#include <stella/stella.h>

#include "../components/game_components.h"

class CollisionSystem : public entityx::System<CollisionSystem> {
public:
  CollisionSystem(int w, int h);
  ~CollisionSystem();
  void update(entityx::EntityManager &es, entityx::EventManager &events,
              entityx::TimeDelta dt) override;

private:
  bool check_collision(entityx::Entity &b1, entityx::Entity &b2);
};
