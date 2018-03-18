#include "game_systems.h"

#include "../components/spatial_component.h"
#include "../components/movement_component.h"


#include <assert.h>

MovementSystem::MovementSystem() {
}

MovementSystem::~MovementSystem() {
}

void MovementSystem::update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt) {
	es.each<SpatialComponent, MovementComponent>([dt](entityx::Entity entity, SpatialComponent &spa, MovementComponent &mov) {
			assert(mov.Direction < 4 && mov.Direction >= 0);
			switch(mov.Direction) {
				case 0:
					if (mov.Eased) {
						spa.y -= mov.Velocity;
						if (spa.y <= spa.ty*spa.h) {
								spa.ty -= 1;
						}
						if (spa.y == (spa.ty + 1)*spa.h) {
							mov.Direction = mov.NewDirection;
							mov.Finished = true;
						}
						else mov.Finished = false;
					}
					else if(mov.Finished) {
						spa.y -= spa.h;
						spa.ty -= 1;
						mov.Direction = mov.NewDirection;
					}
					break;
				case 1:
					if (mov.Eased) {
						spa.x += mov.Velocity;
						if (spa.x > spa.tx*spa.w) {
							spa.tx += 1;
						}
						if (spa.x == spa.tx*spa.w) {
							mov.Direction = mov.NewDirection;
							mov.Finished = true;
						}
						else mov.Finished = false;
					}
					else if (mov.Finished) {
						spa.x += spa.w;
						spa.tx += 1;
						mov.Direction = mov.NewDirection;
					}
					break;
				case 2:
					if (mov.Eased) {
						spa.y += mov.Velocity;
						if (spa.y > spa.ty*spa.h) {
							spa.ty += 1;
						}
						if (spa.y == spa.ty*spa.h) {
							mov.Direction = mov.NewDirection;
							mov.Finished = true;
						}
						else mov.Finished = false;
					}
					else if(mov.Finished) {
						spa.y += spa.h;
						spa.ty += 1;
						mov.Direction = mov.NewDirection;
					}
					break;
				case 3:
					if (mov.Eased) {
						spa.x -= mov.Velocity;
						if (spa.x <= spa.tx*spa.w) {
							spa.tx -= 1;
						}
						if (spa.x == (spa.tx + 1)*spa.w) {
							mov.Direction = mov.NewDirection;
							mov.Finished = true;
						}
						else mov.Finished = false;
					}
					else if (mov.Finished) {
						spa.x -= spa.w;
						spa.tx -= 1;
						mov.Direction = mov.NewDirection;
					}
					break;
			}
	});
}

