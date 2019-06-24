#pragma once

#include <vector>
#include <string>

#include "particles/particles.h"
#include <entityx/entityx.h>

namespace ex = entityx;

namespace stella {
namespace components {
struct ParticleEmitter {
	public:
	  enum Type { FIRE_EMITTER, SNOW_EMITTER };
		std::vector<ex::Entity> Particles;
	  int Velocity;
		stella::graphics::Emitter *Emitter;

		inline ParticleEmitter(Type type, unsigned int max_particles) {
      switch(type) {
        case FIRE_EMITTER:
          this->Velocity = 1;
          this->Emitter = new stella::graphics::FireEmitter(0, 0, max_particles, "fire-particle");
          break;
        case SNOW_EMITTER:
          this->Velocity = 1;
          this->Emitter = new stella::graphics::SnowEmitter(0, 0, max_particles, "snowflake");
          break;
        default:
          this->Velocity = 0;
          this->Emitter = nullptr;
          break;
      }
		}

    inline ParticleEmitter(const ParticleEmitter &copied_emitter) : Particles(copied_emitter.Particles), Velocity(copied_emitter.Velocity) {
      *Emitter = *(copied_emitter.Emitter);
    }

    inline ParticleEmitter & operator= (const ParticleEmitter &p) {
      if (this != &p) {
        Particles = p.Particles;
        Velocity = p.Velocity;
        *Emitter = *(p.Emitter);
      }
      return *this;
    }

		inline ~ParticleEmitter() {
		  if (Emitter)
        delete Emitter;
		}
};
} // namespace components
} // namespace stella

