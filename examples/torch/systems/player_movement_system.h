#pragma once

#include <entityx/entityx.h>
#include <stella/stella.h>

namespace ex = entityx;

class PlayerMovementSystem : public ex::System<PlayerMovementSystem> {
public:
  PlayerMovementSystem(const int &boundx, stella::graphics::Display &display);
  ~PlayerMovementSystem();
  void update(ex::EntityManager &es, ex::EventManager &events,
              ex::TimeDelta dt) override;

private:
  const int BoundX;
	stella::graphics::Display &Display;
};
