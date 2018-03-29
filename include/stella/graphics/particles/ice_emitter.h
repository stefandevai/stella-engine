#pragma once

#include "stella/graphics/particles/emitter.h"

#include <iostream>
#include <cmath>

namespace stella {
namespace graphics {
	class IceEmitter : public Emitter {
		public:
			inline IceEmitter(int posx, int posy, unsigned int max_particles, std::string tex_name) : Emitter(posx, posy, max_particles, tex_name) {
        this->SetMaxLifeRange(std::make_pair(40,80));
        this->SetPositionXRange(std::make_pair(-6,6));
        this->SetPositionYRange(std::make_pair(-6,6));
        this->SetSpeedXRange(std::make_pair(-2.f,2.f));
        this->SetSpeedYRange(std::make_pair(-2.f,2.f));
        this->SetScaleXRange(std::make_pair(0.1f,0.3f));
        this->SetRotationRange(std::make_pair(0.f,45.f));
			}

			inline ~IceEmitter() {
			}

      inline void UpdateParticle(entityx::Entity particle) override {
        auto particle_par = particle.component<ParticleComponent>();

        if (particle.has_component<SpatialComponent>() && particle_par->Life % 2 == 0) {
          auto particle_spa = particle.component<SpatialComponent>();
          particle_spa->x += particle_par->SpeedX;
          particle_spa->y += particle_par->SpeedY;
        }

        ++particle_par->Life;
      }
	};
}}


