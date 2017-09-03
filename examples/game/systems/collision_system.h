#pragma once

#include <vector>
#include <memory>
#include <utility>
#include <unordered_set>
#include <bitset>

#include <entityx/entityx.h>
#include <stella/stella.h>

#include "../components/position_component.h"
#include "../components/body_component.h"

class CollisionSystem : public entityx::System<CollisionSystem> {
	public:
		CollisionSystem(int w, int h);
		~CollisionSystem();
		void update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt) override;

	private:
		static const int PARTITIONS = 250;
		std::bitset<4> collision_direction;
		
		struct Candidate {
			int x, y, width, height;
			entityx::Entity entity;
		};

		std::vector<std::vector<Candidate>> grid;
		unsigned int Width, Height;

		const bool collided(Candidate &c1, Candidate &c2);
		void makeCollisionGrid(entityx::Entity &entity, PositionComponent& pos, BodyComponent& body);
		void resolveCollision(entityx::Entity left, entityx::Entity right);
};

