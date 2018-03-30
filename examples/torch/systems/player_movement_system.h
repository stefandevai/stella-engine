#pragma once

#include <entityx/entityx.h>
#include <stella/stella.h>

class PlayerMovementSystem : public entityx::System<PlayerMovementSystem> {
public:
  PlayerMovementSystem(const int &boundx, stella::graphics::Display &display);
  ~PlayerMovementSystem();
  void update(entityx::EntityManager &es, entityx::EventManager &events,
              entityx::TimeDelta dt) override;

private:
  const int BoundX;
	stella::graphics::Display &Display;
};
