#include "game_systems.h"

#include "../components/position_component.h"
#include "../components/movement_component.h"

MovementSystem::MovementSystem() {
}

MovementSystem::~MovementSystem() {
}

void MovementSystem::update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt) {
	es.each<PositionComponent, MovementComponent>([dt](entityx::Entity entity, PositionComponent &pos, MovementComponent &mov) {
			if (mov.Gravity)
				mov.Acc.y += 9.8f*dt;

			if(std::abs(mov.Vel.x) < mov.MaxVelocity)
				mov.Vel.x +=  mov.Acc.x * dt;

			if(std::abs(mov.Vel.y) < mov.MaxVelocity)
				mov.Vel.y +=  mov.Acc.y * dt;

			if(std::abs(mov.Acc.x) < 1.0f) {
				if (mov.Vel.x > 0.0f) {
					mov.Vel.x -= mov.Drag*dt;
					if (mov.Vel.x < 0.0f) mov.Vel.x = 0.0f;
				}
				else if (mov.Vel.x < 0.0f) {
					mov.Vel.x += mov.Drag*dt;
					if (mov.Vel.x > 0.0f) mov.Vel.x = 0.0f;
				}
			}

			pos.x += (mov.Vel.x);
			pos.y += (mov.Vel.y);
			//std::cout << mov.Vel.x << std::endl;
	});
}

