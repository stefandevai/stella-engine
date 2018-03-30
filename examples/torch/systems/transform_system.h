#pragma once

#include <entityx/entityx.h>

class TransformSystem : public entityx::System<TransformSystem> {
public:
  TransformSystem();
  ~TransformSystem();
  void update(entityx::EntityManager &es, entityx::EventManager &events,
              entityx::TimeDelta dt) override;
};

