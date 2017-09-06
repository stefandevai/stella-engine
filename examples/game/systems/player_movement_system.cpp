#include "player_movement_system.h"

#include <GLFW/glfw3.h>

#include "../components/movement_component.h"
#include "../components/body_component.h"
#include "../components/spatial_component.h"
#include "../components/input_component.h"

PlayerMovementSystem::PlayerMovementSystem(const int &boundx) : BoundX(boundx) {
}

PlayerMovementSystem::~PlayerMovementSystem() {
}

void PlayerMovementSystem::update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt) {
	es.each<MovementComponent, BodyComponent, SpatialComponent, InputComponent>([this](entityx::Entity entity, MovementComponent &mov, BodyComponent &body, SpatialComponent &spa, InputComponent &input) {
		float acceleration = 2.0f;
		float jumpforce = 3.5f;
		
		// Horizontal movement
		if (input.Keys[GLFW_KEY_LEFT]) {
			//std::cout << mov.Acc.x << std::endl;
			mov.Acc.x -= acceleration;
		}
		else if (input.Keys[GLFW_KEY_RIGHT]) {
			mov.Acc.x += acceleration;
		}
		else mov.Acc.x = 0.0f;

		// Jump if body is colliding bottom
		if (body.ColDir.test(0)) {
			mov.Gravity = false;
			mov.Vel.y = 0.0f;
			mov.Acc.y = 0.0f;
			if (input.Keys[GLFW_KEY_UP]) {
				mov.Vel.y = -jumpforce;
			}
		}
		else {
			mov.Gravity = true;
		}

		if (spa.x < 0) spa.x = 0;
		else if (spa.x + spa.w > this->BoundX) spa.x = this->BoundX - spa.w;
	});
}

