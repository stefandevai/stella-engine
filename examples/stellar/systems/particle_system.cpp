#include "particle_system.h"

#include <algorithm>

#include "../components/spatial_component.h"
#include "../components/particle_component.h"
#include "../components/particle_emitter.h"
#include "../components/sprite_component.h"

ParticleSystem::ParticleSystem() {

}

ParticleSystem::~ParticleSystem() {

}

void ParticleSystem::update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt) {
	es.each<SpatialComponent, ParticleEmitter>([this, &es](entityx::Entity entity, SpatialComponent &spa, ParticleEmitter &gen) {
    while (gen.Particles.size() < gen.Emitter->GetMaxParticles()) {
			auto particle = gen.Emitter->Emit(entity, es);
			gen.Particles.push_back(particle);
		}
		if (!gen.Emitter->IsInitialized()) gen.Emitter->Initialize();

		std::vector<std::vector<entityx::Entity>::iterator> particles_to_erase;

		for (auto particle = gen.Particles.begin(); particle != gen.Particles.end(); ++particle) {
			auto par = (*particle).component<ParticleComponent>();
			 
			gen.Emitter->UpdateParticle(*particle);

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

