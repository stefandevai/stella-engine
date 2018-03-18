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
	es.each<MovementComponent, SpatialComponent, InputComponent>([this](entityx::Entity entity, MovementComponent &mov, SpatialComponent &spa, InputComponent &input) {
		if (input.Keys[GLFW_KEY_UP]) {
			mov.NewDirection = 0;
		}
		else if (input.Keys[GLFW_KEY_RIGHT]) {
			mov.NewDirection = 1;
		}
		else if (input.Keys[GLFW_KEY_DOWN]) {
			mov.NewDirection = 2;
		}
		else if (input.Keys[GLFW_KEY_LEFT]) {
			mov.NewDirection = 3;
		}
	});
}

