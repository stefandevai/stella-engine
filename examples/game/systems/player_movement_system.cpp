#include "player_movement_system.h"

#include <GLFW/glfw3.h>

#include "../components/position_component.h"
#include "../components/body_component.h"
#include "../components/input_component.h"

PlayerMovementSystem::PlayerMovementSystem() {
	
}

PlayerMovementSystem::~PlayerMovementSystem() {

}

void PlayerMovementSystem::update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt) {
	es.each<PositionComponent, BodyComponent, InputComponent>([this](entityx::Entity entity, PositionComponent &pos, BodyComponent &body, InputComponent &input) {
		body.Velocity = 4;
		// Horizontal movement
		if (input.Keys[GLFW_KEY_LEFT] || input.Keys[GLFW_KEY_A]) {
			pos.x -= body.Velocity;
		}
		else if (input.Keys[GLFW_KEY_RIGHT] || input.Keys[GLFW_KEY_D]) {
			pos.x += body.Velocity;
		}

		// Vertical movement
		if (input.Keys[GLFW_KEY_UP] || input.Keys[GLFW_KEY_W]) {
			pos.y -= body.Velocity;
		}
		else if (input.Keys[GLFW_KEY_DOWN] || input.Keys[GLFW_KEY_S]) {
			pos.y += body.Velocity;
		}
	});
}

