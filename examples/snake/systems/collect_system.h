#pragma once

#include <entityx/entityx.h>
#include <stella/stella.h>

#include "../events/collision.h"

class CollectSystem : public entityx::System<CollectSystem>, public entityx::Receiver<CollectSystem> {
	public:
		CollectSystem();
		~CollectSystem();

		void configure(entityx::EventManager &event_manager);
		void update(entityx::EntityManager &entities, entityx::EventManager &events, entityx::TimeDelta dt);
		void receive(const Collision &collision);
};

