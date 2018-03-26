#include "particle_system.h"

#include <algorithm>

#include "../components/spatial_component.h"
#include "../components/particle_component.h"
#include "../components/particle_emitter.h"
#include "../components/sprite_component.h"

#define PI 3.14159265

ParticleSystem::ParticleSystem() {

}

ParticleSystem::~ParticleSystem() {

}

void ParticleSystem::update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt) {
	es.each<SpatialComponent, ParticleEmitter>([this, &es](entityx::Entity entity, SpatialComponent &spa, ParticleEmitter &gen) {
		if (!gen.Initialized) {
			for (unsigned int i = 0; i < gen.MaxParticles; ++i) {
				auto particle = es.create();
				unsigned int MaxLife = std::rand() % 41 + 20;
				particle.assign_from_copy<SpatialComponent>(spa);
				particle.assign<ParticleComponent>(MaxLife, 1.0, 1.0, 1.0);
				gen.Particles.push_back(particle);
			}
			gen.Initialized = true;
		}
		else if (gen.Particles.size() < gen.MaxParticles) {
			auto particle = this->CreateParticle(entity, es);
			gen.Particles.push_back(particle);
		}
		std::vector<std::vector<entityx::Entity>::iterator> particles_to_erase;

		for (auto particle = gen.Particles.begin(); particle != gen.Particles.end(); ++particle) {
			auto par = (*particle).component<ParticleComponent>();
			auto particle_spa = (*particle).component<SpatialComponent>();
			 
			stella::graphics::Particle part;
			part.x = particle_spa->x;
			part.y = particle_spa->y;
			part.w = particle_spa->w;
			part.h = particle_spa->h;
			part.spx = par->SpeedX;
			part.spy = par->SpeedY;
			part.life = par->Life;
			gen.Emitter->UpdateParticle(part);
			particle_spa->x = (int)part.x;
			particle_spa->y = (int)part.y;
			particle_spa->w = (int)part.w;
			particle_spa->h = (int)part.h;
			par->SpeedX = part.spx;
			par->SpeedY = part.spy;
			par->Life = part.life;

			//if ((int)par->SpeedX % 3 == 0)
				//particle_spa->x -= cos(par->SpeedX*par->Life*PI/180)*3 - 1;
			//else if ((int)par->SpeedY % 2 == 0)
				//particle_spa->x += sin(par->SpeedX*par->Life*PI/180)*3;

			//particle_spa->y += par->SpeedY;


			//if (par->Life % 5 == 0) {
				//particle_spa->w *= 0.80;
				//particle_spa->w = std::max(1.0f, (float)particle_spa->w);
				//particle_spa->h = particle_spa->w;
			//}
			//++par->Life;

			if (par->Life >= par->MaxLife && par->Alive) {
				particles_to_erase.push_back(particle);
			}
		}
		for (auto& it: particles_to_erase) {
			(*it).destroy();
			gen.Particles.erase(it);
		}
	});
}
	
entityx::Entity ParticleSystem::CreateParticle(entityx::Entity generator, entityx::EntityManager& es) {
		auto gen = generator.component<ParticleEmitter>();
		auto spa = generator.component<SpatialComponent>();

		auto particle = es.create();
		unsigned int MaxLife = std::rand() % 41 + 20;
		double W = 10.0 + static_cast <float>(std::rand())/( static_cast <float> (RAND_MAX/(25.0f - 10.0f)));
		double SpeedX = 10.0 + static_cast <float> (std::rand()) /( static_cast <float> (RAND_MAX/(30.0f-10.0f)));
		double SpeedY = static_cast<float>(std::rand()) / (static_cast <float> (RAND_MAX/3.0));
		int px = spa->x + (-6 + std::rand()/(RAND_MAX/(6 + 6)));

		particle.assign<SpatialComponent>(W, W, px, spa->y);
		particle.assign<ParticleComponent>(MaxLife, W, SpeedX, -SpeedY);
		particle.assign<SpriteComponent>(gen->TextureName);
		return particle;
}
