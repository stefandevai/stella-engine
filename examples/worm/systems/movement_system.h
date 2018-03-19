#pragma once

#include <entityx/entityx.h>
#include <stella/stella.h>

class MovementSystem : public entityx::System<MovementSystem> {
public:
  MovementSystem();
  ~MovementSystem();
  void update(entityx::EntityManager &es, entityx::EventManager &events,
              entityx::TimeDelta dt) override;

private:
};
