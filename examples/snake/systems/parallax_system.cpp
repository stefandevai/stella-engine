#include "parallax_system.h"

#include "../components/parallax_component.h"
#include "../components/spatial_component.h"

ParallaxSystem::ParallaxSystem() {
}

ParallaxSystem::~ParallaxSystem() {
}

void ParallaxSystem::update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt) {
	es.each<SpatialComponent, ParallaxComponent>([this](entityx::Entity entity, SpatialComponent &spa, ParallaxComponent &pll) {
			spa.x += pll.vel;
	});
}


