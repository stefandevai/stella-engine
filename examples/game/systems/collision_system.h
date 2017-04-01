#pragma once

#include <entityx/entityx.h>
#include <stella/stella.h>

class CollisionSystem : public entityx::System<CollisionSystem> {
	public:
		CollisionSystem();
		~CollisionSystem();
		void update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt) override;
};

