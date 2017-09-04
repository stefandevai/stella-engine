#include "collision_system.h"

#include <algorithm>


CollisionSystem::CollisionSystem(int w, int h) {
	Width = w / PARTITIONS + 1;
	Height = h / PARTITIONS + 1;
}

CollisionSystem::~CollisionSystem() {

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
					resolveCollision((*c1).entity, (*c2).entity);
				}
			}
		}
	}
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

const bool CollisionSystem::collided(Candidate &c1, Candidate &c2) {
	bool intersectsX = false, intersectsY = false;
	int intersectXValue = 0, intersectYValue = 0;

	entityx::ComponentHandle<PositionComponent> pos1 = c1.entity.component<PositionComponent>();
	entityx::ComponentHandle<PositionComponent> pos2 = c2.entity.component<PositionComponent>();

	entityx::ComponentHandle<BodyComponent> body1 = c1.entity.component<BodyComponent>();
	entityx::ComponentHandle<BodyComponent> body2 = c2.entity.component<BodyComponent>();

	const int &Ax = pos1->x, &AX = pos1->x + body1->Width, &Ay = pos1->y, &AY = pos1->y + body1->Height,
						&Bx = pos2->x, &BX = pos2->x + body2->Width, &By = pos2->y, &BY = pos2->y + body2->Height;

	body1->ColDir.reset();
	body2->ColDir.reset();

	if (Ay <= BY && AY >= By) {
		intersectsY = true;
	}

	if (Ax <= BX && AX >= Bx) {
		intersectsX = true;
	}
	
	if (!(intersectsX && intersectsY)) {
		collision_direction.reset();
		body1->Colliding = false;
		body2->Colliding = false;
		return false;
	}
	else {
		body1->Colliding = true;
		body2->Colliding = true;
		
		if (Ay < By && AY < BY) {
			intersectYValue = abs(AY - By);
			body1->ColDir.set(0);
			body2->ColDir.set(1);
		}
		else if (Ay > By && AY > BY) {
			intersectYValue = abs(BY - Ay);
			body1->ColDir.set(1);
			body2->ColDir.set(0);
		}
		else intersectYValue = fmin(body1->Height, body2->Height);

		if (Ax < Bx && AX < BX) {
			intersectXValue = abs(AX - Bx);
			if (intersectYValue > intersectXValue) {
				body1->ColDir.reset();
				body2->ColDir.reset();
				
				body1->ColDir.set(2);
				body2->ColDir.set(3);
			}
		}
		else if (Ax > Bx && AX > BX) {
			intersectXValue = abs(BX - Ax);
			if (intersectYValue > intersectXValue) {
				body1->ColDir.reset();
				body2->ColDir.reset();
				
				body1->ColDir.set(3);
				body2->ColDir.set(2);
			}
		}
		return true;
	}
}

void CollisionSystem::resolveCollision(entityx::Entity left, entityx::Entity right) {
	entityx::ComponentHandle<PositionComponent> pos1 = left.component<PositionComponent>();
	entityx::ComponentHandle<PositionComponent> pos2 = right.component<PositionComponent>();

	entityx::ComponentHandle<BodyComponent> body1 = left.component<BodyComponent>();
	entityx::ComponentHandle<BodyComponent> body2 = right.component<BodyComponent>();
	
	// Body1 is Static
	if (body1->Static) {
		// Make sure that body2 is dynamic
		if (!body2->Static) {
			if (body2->ColDir.test(0)) {
				pos2->y -= (pos2->y + body2->Height - pos1->y);
			}
			else if (body2->ColDir.test(1)) {
				pos2->y += (pos1->y + body1->Height - pos2->y);
			}
			else if (body2->ColDir.test(2)) {
				pos2->x -= (pos2->x + body2->Width - pos1->x);
			}
			else if (body2->ColDir.test(3)) {
				pos2->x += (pos1->x + body1->Width - pos2->x);
			}
		}
	}

	// Body2 is Static and body1 is dynamic
	else if (body2->Static) {
		if (body1->ColDir.test(0)) {
			pos1->y -= (pos1->y + body1->Height - pos2->y);
		}
		else if (body1->ColDir.test(1)) {
			pos1->y += (pos2->y + body2->Height - pos1->y);
		}
		else if (body1->ColDir.test(2)) {
			pos1->x -= (pos1->x + body1->Width - pos2->x);
		}
		else if (body1->ColDir.test(3)) {
			pos1->x += (pos2->x + body2->Width - pos1->x);
		}
	}

	// Both bodies are dynamic
	else {

	}
}

