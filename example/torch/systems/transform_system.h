#pragma once

#include <entityx/entityx.h>
namespace ex = entityx;

class TransformSystem : public ex::System<TransformSystem> {
public:
  TransformSystem();
  ~TransformSystem();
  void update(ex::EntityManager &es, ex::EventManager &events,
              ex::TimeDelta dt) override;
};

