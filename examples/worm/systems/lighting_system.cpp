#include "game_systems.h"

#include "../components/light_component.h"
#include "../components/spatial_component.h"

#include <glm/glm.hpp>

LightingSystem::LightingSystem(stella::graphics::Shader *shad) : shader(shad) {
  light_counter = 0;
}

LightingSystem::~LightingSystem() {}

void LightingSystem::update(entityx::EntityManager &es,
                            entityx::EventManager &events,
                            entityx::TimeDelta dt) {
  es.each<SpatialComponent, LightComponent>([this](entityx::Entity entity,
                                                   SpatialComponent &spa,
                                                   LightComponent &light) {
    shader->Enable();
    shader->SetVec2f("lightPos", spa.x + 80, spa.y + 60);
    shader->SetFloat("lightCounter", (float)light_counter);
    shader->SetFloat("lightIntensity", (float)light.Intensity);
  });
  ++light_counter;
}
