#pragma once

#include <vector>
#include <string>

#include "particles/particles.h"
#include <entityx/entityx.h>

namespace ex = entityx;

struct ParticleEmitter {
	public:
	  int Velocity;
	  enum Type { FIRE_EMITTER, SNOW_EMITTER };

		std::vector<ex::Entity> Particles;
		stella::graphics::Emitter *Emitter;

		inline ParticleEmitter(Type type, unsigned int max_particles) {
      switch(type) {
        case FIRE_EMITTER:
          this->Velocity = 1;
          this->Emitter = new stella::graphics::FireEmitter(0, 0, max_particles, "fire-particle");
          break;
        case SNOW_EMITTER:
          this->Velocity = 180;
          this->Emitter = new stella::graphics::SnowEmitter(0, 0, max_particles, "snowflake");
          break;
        default:
          break;
      }
		}
		inline ~ParticleEmitter() {
		  if (Emitter)
        delete Emitter;
		}
};

