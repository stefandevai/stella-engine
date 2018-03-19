#pragma once

#include <entityx/entityx.h>

class ParallaxSystem : public entityx::System<ParallaxSystem> {
public:
  ParallaxSystem();
  ~ParallaxSystem();
  void update(entityx::EntityManager &es, entityx::EventManager &events,
              entityx::TimeDelta dt) override;

private:
};
