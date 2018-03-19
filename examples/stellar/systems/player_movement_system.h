#pragma once

#include <entityx/entityx.h>

class PlayerMovementSystem : public entityx::System<PlayerMovementSystem> {
public:
  PlayerMovementSystem(const int &boundx);
  ~PlayerMovementSystem();
  void update(entityx::EntityManager &es, entityx::EventManager &events,
              entityx::TimeDelta dt) override;

private:
  const int BoundX;
};
