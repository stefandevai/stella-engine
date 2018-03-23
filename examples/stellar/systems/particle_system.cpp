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

void ParticleSystem::update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt) {

	es.each<SpatialComponent, ParticleGenerator>([this, &es](entityx::Entity entity, SpatialComponent &spa, ParticleGenerator &gen) {
		if (!gen.Initialized) {
			for (unsigned int i = 0; i < gen.MaxParticles; ++i) {
				auto particle = es.create();
				unsigned int MaxLife = std::rand() % 41 + 20;
				particle.assign<SpatialComponent>(1, 1, spa.x, spa.y);
				particle.assign<ParticleComponent>(MaxLife, 1.0, 1.0, 1.0);
				gen.Particles.push_back(particle);
			}
			gen.Initialized = true;
		}
		else if (gen.Particles.size() < gen.MaxParticles) {
			auto particle = this->CreateParticle(entity, es);
			gen.Particles.push_back(particle);
		}

		for (auto particle = gen.Particles.begin(); particle != gen.Particles.end(); ++particle) {
			auto par = (*particle).component<ParticleComponent>();
			auto particle_spa = (*particle).component<SpatialComponent>();
			 
			//particle_spa->x += par->SpeedX;
			//particle_spa->y += par->SpeedY;
			particle_spa->x += cos(par->Life);
			particle_spa->y += par->SpeedY;
			particle_spa->w = (int)par->W;
			particle_spa->h = (int)par->H;
			++par->Life;

			if (par->Life >= par->MaxLife && par->Alive) {
				(*particle).destroy();
				gen.Particles.erase(particle);
			}
		}
	});
}
	
entityx::Entity ParticleSystem::CreateParticle(entityx::Entity generator, entityx::EntityManager& es) {
		auto gen = generator.component<ParticleGenerator>();
		auto spa = generator.component<SpatialComponent>();

		auto particle = es.create();
		unsigned int MaxLife = std::rand() % 41 + 20;
		//unsigned int MaxLife = std::rand() % 21 + 50;
		double W = 5.0 + static_cast <float>(std::rand())/( static_cast <float> (RAND_MAX/(16.0f - 5.0f)));
		double SpeedX = -3.0 + static_cast <float> (std::rand()) /( static_cast <float> (RAND_MAX/(3.0f+3.0f)));
		double SpeedY = static_cast<float>(std::rand()) / (static_cast <float> (RAND_MAX/3.0));

		particle.assign<SpatialComponent>(W, W, spa->x, spa->y);
		particle.assign<ParticleComponent>(MaxLife, W, SpeedX, -SpeedY);
		particle.assign<SpriteComponent>(gen->TextureName);
		return particle;
}
