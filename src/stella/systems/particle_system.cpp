#include <algorithm>

#include "stella/components.h"
#include "stella/systems/particle_system.h"

namespace stella {
namespace systems {
ParticleSystem::ParticleSystem() {

}

ParticleSystem::~ParticleSystem() {

}

void ParticleSystem::update(ex::EntityManager &es, ex::EventManager &events, ex::TimeDelta dt) {
	es.each<components::ParticleEmitter>([this, &es](ex::Entity entity, components::ParticleEmitter &gen) {
    if(++this->Timer % gen.Velocity == 0) {
      auto particle = gen.Emitter->Emit(entity, es);
      gen.Particles.push_back(particle);

      if (this->Timer > 1000000) this->Timer = 0;
    }

		std::vector<std::vector<ex::Entity>::iterator> particles_to_erase;

		for (auto particle = gen.Particles.begin(); particle != gen.Particles.end(); ++particle) {
      if ((*particle).valid()) {
        auto par = (*particle).component<components::ParticleComponent>();
         
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
} // namespace systems
} // namespace stella

