#include "game_systems.h"

#include "../components/spatial_component.h"
#include "../components/movement_component.h"
#include "../components/timer_component.h"

#include <assert.h>

MovementSystem::MovementSystem() {
}

MovementSystem::~MovementSystem() {
}

void MovementSystem::update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt) {
	es.each<SpatialComponent, MovementComponent, TimerComponent>([dt](entityx::Entity entity, SpatialComponent &spa, MovementComponent &mov, TimerComponent &tmr) {
			int vel = 1;

			assert(mov.Direction < 4 && mov.Direction >= 0);
			switch(mov.Direction) {
				case 0:
					spa.y -= vel;
					if (spa.y <= spa.ty*spa.h) {
						spa.ty -= 1;
					}
					if (spa.y == (spa.ty + 1)*spa.h) {
						mov.Direction = mov.NewDirection;
					}
					break;
				case 1:
					spa.x += vel;
					if (spa.x > spa.tx*spa.w) {
						spa.tx += 1;
					}
					if (spa.x == spa.tx*spa.w) {
						mov.Direction = mov.NewDirection;
					}
					break;
				case 2:
					spa.y += vel;
					if (spa.y > spa.ty*spa.h) {
						spa.ty += 1;
					}
					if (spa.y == spa.ty*spa.h) {
						mov.Direction = mov.NewDirection;
					}
					break;
				case 3:
					spa.x -= vel;
					if (spa.x <= spa.tx*spa.w) {
						spa.tx -= 1;
					}
					if (spa.x == (spa.tx + 1)*spa.w) {
						mov.Direction = mov.NewDirection;
					}
					break;
			}
			if (tmr.frame > 1000000) tmr.frame = 0;
			tmr.frame++;
	});
}

