#include "game_systems.h"

#include "../components/spatial_component.h"
#include "../components/movement_component.h"

MovementSystem::MovementSystem() {
}

MovementSystem::~MovementSystem() {
}

void MovementSystem::update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt) {
	es.each<SpatialComponent, MovementComponent>([dt](entityx::Entity entity, SpatialComponent &spa, MovementComponent &mov) {
			if(mov.Gravity && mov.Acc.y < 10.0f)
				mov.Acc.y += 15.0f*dt;

			if (mov.Acc.x > 10.0f) mov.Acc.x = 10.0f;
			else if (mov.Acc.x < -10.0f) mov.Acc.x = -10.0f;

			if(std::abs(mov.Vel.y) < 8.0f)
				mov.Vel.y +=  mov.Acc.y * dt;
			if(std::abs(mov.Vel.x) < mov.MaxVelocity)
				mov.Vel.x += mov.Acc.x * dt;

			//if(std::abs(mov.Vel.x) < 0.1f) mov.Vel.x = 0.0f;
			//if(std::abs(mov.Acc.x) == 0.0f) mov.Vel.x *= powf(0.2f, dt);

			if(std::abs(mov.Acc.x) == 0.0f) {
				if (mov.Vel.x > 0.0f) {
					mov.Vel.x -= 9.0f*dt;
					if (mov.Vel.x < 0.0f) mov.Vel.x = 0.0f;
				}
				else if (mov.Vel.x < 0.0f) {
					mov.Vel.x += 9.0f*dt;
					if (mov.Vel.x > 0.0f) mov.Vel.x = 0.0f;
				}
			}


			//std::cout << mov.Vel.x << std::endl;
			if (mov.Vel.x > 0.0f) spa.x += std::ceil(mov.Vel.x);
			else spa.x += std::floor(mov.Vel.x);
			spa.y += (mov.Vel.y);
	});
}

