#include "player_movement_system.h"

#include <GLFW/glfw3.h>

#include "../components/position_component.h"
#include "../components/input_component.h"

PlayerMovementSystem::PlayerMovementSystem(int boundx, int boundy) : BoundX(boundx), BoundY(boundy) {
	
}

PlayerMovementSystem::~PlayerMovementSystem() {

}

void PlayerMovementSystem::update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt) {
	es.each<PositionComponent, InputComponent>([this](entityx::Entity entity, PositionComponent &pos, InputComponent &input) {
			// Horizontal movement
			if (input.Keys[GLFW_KEY_LEFT] || input.Keys[GLFW_KEY_A]) {
				if (pos.x >= 0)
					pos.x -= 4;
			}
			else if (input.Keys[GLFW_KEY_RIGHT] || input.Keys[GLFW_KEY_D]) {
				if (pos.x + 80 < BoundX)
					pos.x += 4;
			}

			// Vertical movement
			if (input.Keys[GLFW_KEY_UP] || input.Keys[GLFW_KEY_W]) {
				if (pos.y >= 0)
					pos.y -= 4;
			}
			else if (input.Keys[GLFW_KEY_DOWN] || input.Keys[GLFW_KEY_S]) {
				if (pos.y + 60 < BoundY)
					pos.y += 4;
			}
	});
}

