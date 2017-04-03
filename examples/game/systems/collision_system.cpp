#include "collision_system.h"

#include <algorithm>

#include "../components/position_component.h"
#include "../components/body_component.h"

CollisionSystem::CollisionSystem(int w, int h) {
	Width = w / PARTITIONS + 1;
	Height = h / PARTITIONS + 1;
}

CollisionSystem::~CollisionSystem() {

}

void CollisionSystem::configure(entityx::EventManager &events) {
	events.subscribe<CollisionEvent>(*this);
}

void CollisionSystem::receive(const CollisionEvent &collision) {
	current_collisions.push_back(std::make_pair(collision.Left, collision.Right));
}

void CollisionSystem::update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt) {
	grid.clear();
	grid.resize(Width * Height);

	es.each<PositionComponent, BodyComponent>([this](entityx::Entity entity, PositionComponent &pos, BodyComponent &body) {
		unsigned int left = pos.x/PARTITIONS, 
								 right = (pos.x + body.Width)/PARTITIONS,
								 top = pos.y/PARTITIONS, 
								 bottom = (pos.y + body.Height)/PARTITIONS;

		Candidate candidate{pos.x, pos.y, body.Width, body.Height, entity};

		unsigned int slots[4] = {
			left + top*Width,
			right + top*Width,
			left + bottom*Width,
			right + bottom*Width
		};

		grid[slots[0]].push_back(candidate);

		if (slots[0] != slots[1]) {
			unsigned int mins = std::min(slots[0], slots[1]), maxs = std::max(slots[0], slots[1]);

			for (unsigned int i = mins+1; i <= maxs; ++i) {
				grid[i].push_back(candidate);
			}
		}

		if (slots[1] != slots[2]) {
			unsigned int mins = std::min(slots[0], slots[2]), maxs = std::max(slots[0], slots[2]);
			
			for (unsigned int i = mins+Width; i <= maxs; i += Width)
				if (i <= maxs) grid[i].push_back(candidate);
		} 
		if (slots[2] != slots[3]) {
			for (unsigned int i = slots[2]; i <= slots[3]; ++i)
				grid[i].push_back(candidate);

			for (unsigned int i = slots[1]; i <= slots[3]; i += Width)
				if (i <= slots[3]) grid[i].push_back(candidate);
		} 
	});

	for (auto candidates = grid.begin(); candidates != grid.end(); ++candidates) {
		for (auto c1 = (*candidates).begin(); c1 != (*candidates).end(); ++c1) {
			for (auto c2 = c1; c2 != (*candidates).end(); ++c2) {
				if ((*c1).entity != (*c2).entity && collided(*c1, *c2)) {
					events.emit<CollisionEvent>((*c1).entity, (*c2).entity);
				}
			}
		}
	}

	// Resolving collision
	for (auto col : current_collisions) {
		entityx::ComponentHandle<PositionComponent> pos1 = col.first.component<PositionComponent>();
		entityx::ComponentHandle<PositionComponent> pos2 = col.second.component<PositionComponent>();

		entityx::ComponentHandle<BodyComponent> body1 = col.first.component<BodyComponent>();
		entityx::ComponentHandle<BodyComponent> body2 = col.second.component<BodyComponent>();

		if (pos1->y + body1->Height > pos2->y && pos1->y + body1->Height < pos2->y + body2->Height) pos1->y -= 10;
	}
}

bool CollisionSystem::collided(const Candidate &c1, const Candidate &c2) {
	bool intersectsX = false, intersectsY = false;

	// C1 bottom and C2 top
	if (c1.y + c1.height > c2.y && c1.y + c1.height <= c2.y + c2.height) {
		//std::cout << "C1/C2 Intersetcs Y at " << c1.y + c1.height << "," << c2.y << std::endl;
		//std::cout << "C1/C2 Intersetcs Y at " << c1.y << "," << c2.y << std::endl;
		intersectsY = true;
	}

	// C1 top and C2 bottom
	else if (c1.y < c2.y + c2.height && c1.y >= c2.y) {
		//std::cout << "C2/C1 Intersetcs Y at " << c1.y << "," << c2.y + c2.height << std::endl;
		//std::cout << "C2/C1 Intersetcs Y at " << c1.y << "," << c2.y << std::endl;
		intersectsY = true;
	}

	// C1 right and C2 left
	if (c1.x + c1.width > c2.x && c1.x + c1.width < c2.x + c2.width) {
		//std::cout << "C1/C2 Intersetcs X at " << c1.x + c1.width << "," << c2.x << std::endl;
		//std::cout << "C1/C2 Intersetcs X at " << c1.x << "," << c2.x << std::endl;
		intersectsX = true;
	}
	
	// C1 left and C2 right
	else if (c1.x < c2.x + c2.width && c1.x >= c2.x) {
		//std::cout << "C2/C1 Intersetcs X at " << c1.x << "," << c2.x + c2.width << std::endl;
		//std::cout << "C2/C1 Intersetcs X at " << c1.x << "," << c2.x << std::endl;
		intersectsX = true;
	}

	return (intersectsX && intersectsY);
}


