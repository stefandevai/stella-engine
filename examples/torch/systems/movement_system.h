#pragma once

#include <entityx/entityx.h>
#include <stella/stella.h>

namespace ex = entityx;

class MovementSystem : public ex::System<MovementSystem> {
public:
  MovementSystem();
  ~MovementSystem();
  void update(ex::EntityManager &es, ex::EventManager &events,
              ex::TimeDelta dt) override;

private:
};
