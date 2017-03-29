#include "game_systems.h"

#include "../components/position_component.h"
#include "../components/light_component.h"

#include <glm/glm.hpp>

LightingSystem::LightingSystem(stella::graphics::Shader *shad) : shader(shad) {
	light_counter = 0;
}

LightingSystem::~LightingSystem() {

}

void LightingSystem::update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt) {
	es.each<PositionComponent, LightComponent>([this](entityx::Entity entity, PositionComponent &pos, LightComponent &light) {
		shader->Enable();
		shader->SetVec2f("lightPos", pos.x + 80, pos.y + 60);
    shader->SetFloat("lightCounter", (float)light_counter);
    shader->SetFloat("lightIntensity", (float)light.Intensity);
	});
	++light_counter;
}

