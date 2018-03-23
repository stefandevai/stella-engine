#include "particle_system.h"

#include <algorithm>

#include "../components/spatial_component.h"
#include "../components/particle_component.h"
#include "../components/particle_generator.h"
#include "../components/sprite_component.h"

#define PI 3.14159265

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
			//particle_spa->x += sin(par->SpeedX*par->Life*PI/180)*5;
			//particle_spa->x += cos(11*par->Life*PI/180)*4;
			if ((int)par->SpeedY % 2 == 0)
				particle_spa->x += cos(-(par->SpeedX)*par->Life*PI/180)*4 + 1;
			else
				particle_spa->x += sin(par->SpeedX*par->Life*PI/180)*4 + 1;

			particle_spa->y += par->SpeedY;


			if (par->Life % 5 == 0) {
				particle_spa->w *= 0.85;
				particle_spa->w = std::max(1.0f, (float)particle_spa->w);
				particle_spa->h = particle_spa->w;
			}
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
		double W = 10.0 + static_cast <float>(std::rand())/( static_cast <float> (RAND_MAX/(30.0f - 8.0f)));
		double SpeedX = 10.0 + static_cast <float> (std::rand()) /( static_cast <float> (RAND_MAX/(30.0f-10.0f)));
		double SpeedY = static_cast<float>(std::rand()) / (static_cast <float> (RAND_MAX/3.0));
		//unsigned int MaxLife = 20;
		//double W = 30.0;
		//double SpeedX = 1.0;
		//double SpeedY = 5.0;

		particle.assign<SpatialComponent>(W, W, spa->x, spa->y);
		particle.assign<ParticleComponent>(MaxLife, W, SpeedX, -SpeedY);
		particle.assign<SpriteComponent>(gen->TextureName);
		return particle;
}
