#pragma once

#include <vector>
#include <string>

#include "particles/particles.h"
#include <entityx/entityx.h>

struct ParticleEmitter {
	public:
	  int Velocity;
	  enum Type { FIRE_EMITTER, SNOW_EMITTER, ICE_EMITTER };

		std::vector<entityx::Entity> Particles;
		stella::graphics::Emitter *Emitter;

		inline ParticleEmitter(Type type, unsigned int max_particles) {
      switch(type) {
        case FIRE_EMITTER:
          this->Velocity = 1;
          this->Emitter = new stella::graphics::FireEmitter(0, 0, max_particles, "fire-particle");
          break;
        case ICE_EMITTER:
          this->Velocity = 2;
          this->Emitter = new stella::graphics::IceEmitter(0, 0, max_particles, "ice-particle");
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

