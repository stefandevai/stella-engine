#include "player_movement_system.h"

#include <GLFW/glfw3.h>

#include "../components/movement_component.h"
#include "../components/body_component.h"
#include "../components/input_component.h"

PlayerMovementSystem::PlayerMovementSystem() {
	
}

PlayerMovementSystem::~PlayerMovementSystem() {

}

void PlayerMovementSystem::update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt) {
	es.each<MovementComponent, BodyComponent, InputComponent>([dt](entityx::Entity entity, MovementComponent &mov, BodyComponent &body, InputComponent &input) {
		float accel = 30.0f;
		
		// Horizontal movement
		if (input.Keys[GLFW_KEY_LEFT] || input.Keys[GLFW_KEY_A]) {
			mov.Acc.x = -accel;
			if (body.ColDir.test(3)) mov.Vel.x = 0.0f;
		}
		else if (input.Keys[GLFW_KEY_RIGHT] || input.Keys[GLFW_KEY_D]) {
			mov.Acc.x = accel;
			if (body.ColDir.test(2)) mov.Vel.x = 0.0f;
		}
		else mov.Acc.x = 0.0f;

		// Vertical movement
		if (input.Keys[GLFW_KEY_UP] || input.Keys[GLFW_KEY_W]) {
			mov.Acc.y = -accel;
			if (body.ColDir.test(1)) mov.Vel.y = 0.0f;
		}
		else if (input.Keys[GLFW_KEY_DOWN] || input.Keys[GLFW_KEY_S]) {
			mov.Acc.y = accel;

			// Gravity takes part if player is not colliding down
			if (body.ColDir.test(0)) {
				mov.Acc.y = 0.0f;
				mov.Vel.y = 0.0f;
				mov.Gravity = false;
			}
			else mov.Gravity = true;
		}
	});
}

