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

		int velox1, velox2, veloy1, veloy2;
		velox1 = velox2 = veloy1 = veloy2 = 4;

		if (body.ColDir.test(0)) veloy1 = 0;
		else if (body.ColDir.test(1)) veloy2 = 0;
		if (body.ColDir.test(2)) velox1 = 0;
		else if (body.ColDir.test(3)) velox2 = 0;
		
		// Horizontal movement
		if (input.Keys[GLFW_KEY_LEFT] || input.Keys[GLFW_KEY_A]) {
			pos.x -= velox2;
		}
		else if (input.Keys[GLFW_KEY_RIGHT] || input.Keys[GLFW_KEY_D]) {
			pos.x += velox1;
		}

		// Vertical movement
		if (input.Keys[GLFW_KEY_UP] || input.Keys[GLFW_KEY_W]) {
			pos.y -= veloy2;
		}
		else if (input.Keys[GLFW_KEY_DOWN] || input.Keys[GLFW_KEY_S]) {
			pos.y += veloy1;
		}
	});
}

