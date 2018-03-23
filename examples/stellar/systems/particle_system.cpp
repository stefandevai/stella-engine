#include "particle_system.h"

#include <algorithm>

#include "../components/spatial_component.h"
#include "../components/particle_component.h"
#include "../components/particle_generator.h"
#include "../components/sprite_component.h"

ParticleSystem::ParticleSystem() {

}

ParticleSystem::~ParticleSystem() {

}

void ParticleSystem::update(entityx::EntityManager &es,
                            entityx::EventManager &events,
                            entityx::TimeDelta dt) {

	es.each<SpatialComponent, ParticleGenerator>([this, &es](entityx::Entity entity, SpatialComponent &spa, ParticleGenerator &gen) {
		if (!gen.Initialized) {
			for (int i = 0; i < gen.MaxParticles; ++i) {
				this->CreateParticle(es);
			}
			gen.Initialized = true;
		}
	});


  es.each<SpatialComponent, ParticleComponent>([this, &es](entityx::Entity entity,
                                                     SpatialComponent &spa,
                                                     ParticleComponent &par) {
		spa.x += par.SpeedX;
		spa.y += par.SpeedY;
		spa.w = (int)par.W;
		spa.h = (int)par.H;
		++par.Life;

		if (par.Life >= par.MaxLife && par.Alive) {
			entity.destroy();
			this->CreateParticle(es);
		}
  });
}
	
void ParticleSystem::CreateParticle(entityx::EntityManager& es) {
		auto particle = es.create();
		unsigned int MaxLife = std::rand() % 41 + 20;
		double W = 5.0 + static_cast <float>(std::rand())/( static_cast <float> (RAND_MAX/(16.0f - 5.0f)));
		double SpeedX = -3.0 + static_cast <float> (std::rand()) /( static_cast <float> (RAND_MAX/(3.0f+3.0f)));
		double SpeedY = static_cast<float>(std::rand()) / (static_cast <float> (RAND_MAX/3.0));
		particle.assign<SpatialComponent>(W, W, 300, 200);
		particle.assign<ParticleComponent>(MaxLife, W, SpeedX, -SpeedY);
		particle.assign<SpriteComponent>("fire-particle");
}
