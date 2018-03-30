#pragma once

#include <entityx/entityx.h>

class TorchSystem : public entityx::System<TorchSystem> {
public:
  TorchSystem(entityx::Entity player);
  ~TorchSystem();
  void update(entityx::EntityManager &es, entityx::EventManager &events,
              entityx::TimeDelta dt) override;

private:
  entityx::Entity Player;
};


