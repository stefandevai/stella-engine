#pragma once

#include <entityx/entityx.h>
#include "../events/collision.h"

class TorchSystem : public entityx::System<TorchSystem>, public entityx::Receiver<TorchSystem> {
public:
  TorchSystem(entityx::Entity player);
  ~TorchSystem();
  void configure(entityx::EventManager &events);
  void update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt) override;
  void receive(const Collision &collision);

private:
  entityx::Entity Player;
};


