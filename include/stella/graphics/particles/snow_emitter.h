#pragma once

#include "stella/graphics/particles/emitter.h"

#include <iostream>
#include <cmath>

namespace stella {
namespace graphics {
	class SnowEmitter : public Emitter {
		public:
			inline SnowEmitter(int posx, int posy, unsigned int max_particles, std::string tex_name) : Emitter(posx, posy, max_particles, tex_name) {
        this->SetMaxLifeRange(std::make_pair(500,600));
        this->SetPositionXRange(std::make_pair(-384,384));
        this->SetPositionYRange(std::make_pair(0,0));
        this->SetSpeedXRange(std::make_pair(1.0f,4.0f));
        this->SetSpeedYRange(std::make_pair(1.f,4.f));
        this->SetScaleXRange(std::make_pair(0.1f,0.5f));
        this->SetRotationRange(std::make_pair(0.f,90.f));
			}

			inline ~SnowEmitter() {
			}

      inline void UpdateParticle(entityx::Entity particle) override {
        auto particle_par = particle.component<ParticleComponent>();

        if (particle.has_component<PositionComponent>() && particle_par->Life % 1 == 0) {
          auto particle_pos = particle.component<PositionComponent>();
          particle_pos->x += particle_par->SpeedX;
          particle_pos->y += particle_par->SpeedY;
        }

        ++particle_par->Life;
      }
	};
}}


