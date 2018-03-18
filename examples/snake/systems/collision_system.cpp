#include "collision_system.h"

CollisionSystem::CollisionSystem(int w, int h) {
}

CollisionSystem::~CollisionSystem() {
}

void CollisionSystem::update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt) {
	static_bodies.clear();
	dynamic_bodies.clear();

	es.each<SpatialComponent, BodyComponent>([this](entityx::Entity entity, SpatialComponent &spa, BodyComponent &body) {
			if (body.IsStatic)
				static_bodies.push_back(entity);
			else
				dynamic_bodies.push_back(entity);
	});

	for (auto b1 : dynamic_bodies)
		for (auto b2 : static_bodies)
			if (check_collision(&b1, &b2))
				std::cout << "Collided!" << std::endl;
}

bool CollisionSystem::check_collision(entityx::Entity *b1, entityx::Entity *b2) {
	auto spa1 = b1->component<SpatialComponent>();
	auto spa2 = b2->component<SpatialComponent>();

	bool colX = spa1->x + spa1->w > spa2->x && spa2->x + spa2->w > spa1->x;
	bool colY = spa1->y + spa1->h > spa2->y && spa2->y + spa2->h > spa1->y;

	return colX && colY;
}

