#include "particle_system.h"

#include <algorithm>

#include "../components/game_components.h"

ParticleSystem::ParticleSystem() {

}

ParticleSystem::~ParticleSystem() {

}

void ParticleSystem::update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt) {
	es.each<ParticleEmitter>([this, &es](entityx::Entity entity, ParticleEmitter &gen) {
    if(++this->Timer % gen.Velocity == 0) {
      auto particle = gen.Emitter->Emit(entity, es);
      gen.Particles.push_back(particle);

      if (this->Timer > 1000000) this->Timer = 0;
    }

		std::vector<std::vector<entityx::Entity>::iterator> particles_to_erase;

		for (auto particle = gen.Particles.begin(); particle != gen.Particles.end(); ++particle) {
      if ((*particle).valid()) {
        auto par = (*particle).component<ParticleComponent>();
         
        gen.Emitter->UpdateParticle(*particle);

        if (par->Life >= par->MaxLife && par->Alive) {
          particles_to_erase.push_back(particle);
        }
      }
      else {
        particles_to_erase.push_back(particle);
      }
		}
		for (auto& it: particles_to_erase) {
		  if ((*it).valid())
        (*it).destroy();
			gen.Particles.erase(it);
		}
	});
}

