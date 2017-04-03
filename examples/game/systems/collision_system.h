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

struct CollisionEvent {
	CollisionEvent(entityx::Entity left, entityx::Entity right) : Left(left), Right(right) {}
	entityx::Entity Left, Right;
};

class CollisionSystem : public entityx::System<CollisionSystem>, public entityx::Receiver<CollisionSystem> {
	public:
		CollisionSystem(int w, int h);
		~CollisionSystem();
		void update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt) override;

		void configure(entityx::EventManager &events) override;
		void receive(const CollisionEvent &collision);

	private:
		static const int PARTITIONS = 200;
		std::bitset<4> collision_direction;
		
		struct Candidate {
			int x, y, width, height;
			entityx::Entity entity;
		};

		std::vector<std::vector<Candidate>> grid;
		unsigned int Width, Height;
		std::vector<std::pair<entityx::Entity, entityx::Entity>> current_collisions;

		bool collided(const Candidate &c1, const Candidate &c2);
		void makeCollisionGrid(entityx::Entity &entity, PositionComponent& pos, BodyComponent& body);
		void resolveCollisions();
};

