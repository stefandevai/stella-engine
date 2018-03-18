#pragma once

#include <vector>
#include <memory>
#include <utility>
#include <unordered_set>
#include <bitset>

#include <entityx/entityx.h>
#include <stella/stella.h>

#include "../components/spatial_component.h"
#include "../components/body_component.h"

class CollisionSystem : public entityx::System<CollisionSystem> {
	public:
		CollisionSystem(int w, int h);
		~CollisionSystem();
		void update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt) override;

	private:
		std::vector<entityx::Entity> static_bodies;
		std::vector<entityx::Entity> dynamic_bodies;

		bool check_collision(entityx::Entity *b1, entityx::Entity *b2);
};

