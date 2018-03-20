#pragma once

#include <entityx/entityx.h>
#include <stella/stella.h>

#include "../events/collision.h"

#include "../components/collectible_component.h"

class CollectSystem : public entityx::System<CollectSystem>,
                      public entityx::Receiver<CollectSystem> {
public:
  CollectSystem();
  ~CollectSystem();

  void configure(entityx::EventManager &events);
  void update(entityx::EntityManager &entities, entityx::EventManager &events,
              entityx::TimeDelta dt);
  void receive(const Collision &collision);

private:
	CollectibleType Collectible;
};
