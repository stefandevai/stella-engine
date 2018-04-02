#pragma once

#include "emitter.h"

#include <iostream>
#include <cmath>

namespace stella {
namespace graphics {
	class SnowEmitter : public Emitter {
		public:
			inline SnowEmitter(int posx, int posy, unsigned int max_particles, std::string tex_name) : Emitter(posx, posy, max_particles, tex_name) {
        this->SetMaxLifeRange(std::make_pair(500,600));
        this->SetPositionXRange(std::make_pair(0.0f,32.0f));
        this->SetPositionYRange(std::make_pair(0,0));
        this->SetSpeedXRange(std::make_pair(1.0f,4.0f));
        this->SetSpeedYRange(std::make_pair(1.f,1.f));
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
			inline  entityx::Entity Emit(entityx::Entity generator, entityx::EntityManager& es) override {
        auto pos = generator.component<PositionComponent>();
        auto dim = generator.component<DimensionComponent>();

        auto particle = es.create();
        
        unsigned int max_life = this->GetRandomValue<unsigned int>(this->Data.MaxLifeRange);
        int px = pos->x + dim->w*((int)this->GetRandomValue<int>(this->Data.PositionXRange)%(int)dim->w);
        double speedy = this->GetRandomValue<float>(this->Data.SpeedYRange, true);
        float rotation = this->GetRandomValue<float>(this->Data.RotationRange);

        particle.assign<PositionComponent>(px, pos->y);
        particle.assign<DimensionComponent>(dim->w, dim->h);
        particle.assign<ParticleComponent>(max_life, 16.f, 0.f, speedy, 1);
        particle.assign<SpriteComponent>(this->TextureName);
        particle.assign<TransformComponent>(rotation, glm::vec2(1.f, 1.f));
        return particle;
			}
	};
}}


