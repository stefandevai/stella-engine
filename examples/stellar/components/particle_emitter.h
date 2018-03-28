#include <vector>
#include <string>

#include <stella/particles.h>
#include <entityx/entityx.h>

struct ParticleEmitter {
	public:
	  enum Type { FIRE_EMITTER };

		std::vector<entityx::Entity> Particles;
		stella::graphics::Emitter *Emitter;

		inline ParticleEmitter(Type type, unsigned int max_particles) {
      switch(type) {
        case FIRE_EMITTER:
          this->Emitter = new stella::graphics::FireEmitter(0, 0, max_particles, "fire-particle");
          break;
        default:
          this->Emitter = new stella::graphics::FireEmitter(0, 0, max_particles, "fire-particle");
          break;
      }
		}
		inline ~ParticleEmitter() {
		  if (Emitter)
        delete Emitter;
		}
};

