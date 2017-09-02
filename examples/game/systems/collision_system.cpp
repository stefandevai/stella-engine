#include "collision_system.h"

#include <algorithm>


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
	current_collisions.push_back(std::make_pair(collision, collision_direction));
	collision_direction.reset();
}

void CollisionSystem::update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt) {
	// Clear collision grid each frame
	grid.clear();
	grid.resize(Width * Height);

	es.each<PositionComponent, BodyComponent>([this](entityx::Entity entity, PositionComponent &pos, BodyComponent &body) {
			// Build collision grid
			makeCollisionGrid(entity, pos, body);
	});

	// Check for collosions on all candidates
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
	resolveCollisions();
}

void CollisionSystem::makeCollisionGrid(entityx::Entity &entity, PositionComponent& pos, BodyComponent& body) {
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
}

void CollisionSystem::resolveCollisions() {
	for (auto col : current_collisions) {
		entityx::ComponentHandle<PositionComponent> pos1 = col.first.Left.component<PositionComponent>();
		entityx::ComponentHandle<PositionComponent> pos2 = col.first.Right.component<PositionComponent>();

		entityx::ComponentHandle<BodyComponent> body1 = col.first.Left.component<BodyComponent>();
		entityx::ComponentHandle<BodyComponent> body2 = col.first.Right.component<BodyComponent>();

		const std::bitset<4> &direction = col.second;

		// Top collision
		if (direction.test(0)) {
			pos1->y -= (pos1->y + body1->Height - pos2->y);
		}
		// Bottom collision
		else if (direction.test(1)) {
			pos1->y += (pos2->y + body2->Height - pos1->y);
		}
		// Left collision
		else if (direction.test(2)) {
			pos1->x -= (pos1->x + body1->Width - pos2->x);
		}
		// Right collision
		else if (direction.test(3)) {
			pos1->x += (pos2->x + body2->Width - pos1->x);
		}
	}
	current_collisions.clear();
}

bool CollisionSystem::collided(const Candidate &c1, const Candidate &c2) {
	bool intersectsX = false, intersectsY = false;
	int intersectXValue = 0, intersectYValue = 0;
	const int &Ax = c1.x, &AX = c1.x + c1.width, &Ay = c1.y, &AY = c1.y + c1.height,
						&Bx = c2.x, &BX = c2.x + c2.width, &By = c2.y, &BY = c2.y + c2.height;

	// Check for collisions
	// C1 bottom and C2 top
	if (AY > By && AY <= BY) {
		intersectsY = true;
		intersectYValue = abs(AY - By);
		collision_direction.set(0);
	}

	// C1 top and C2 bottom
	else if (Ay < BY && Ay >= By) {
		intersectsY = true;
		intersectYValue = abs(BY - Ay);
		collision_direction.set(1);
	}

	// C1 right and C2 left
	if (AX > Bx && AX < BX) {
		intersectsX = true;
		intersectXValue = abs(AX - Bx);

		if (intersectYValue - intersectXValue > 0) {
			collision_direction.reset();
			collision_direction.set(2);
		}
	}
	
	// C1 right and C2 left
	else if (Ax < BX && Ax >= Bx) {
		intersectsX = true;
		intersectXValue = abs(BX - Ax);

		if (intersectYValue - intersectXValue > 0) {
			collision_direction.reset();
			collision_direction.set(3);
		}
	}
	
	return (intersectsX && intersectsY);
}

