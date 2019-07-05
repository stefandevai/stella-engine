#pragma once

#include "emitter.h"

#include <iostream>
#include <cmath>

namespace stella {
namespace graphics {
	class SnowEmitter : public Emitter {
		public:
			inline SnowEmitter(int posx, int posy, unsigned int max_particles, const std::string& tex_name) : Emitter(posx, posy, max_particles, tex_name) {
        this->SetMaxLifeRange(std::make_pair(100,200));
        this->SetPositionXRange(std::make_pair(0.0f,32.0f));
        this->SetPositionYRange(std::make_pair(0,0));
        this->SetSpeedXRange(std::make_pair(-10.0f,-5.0f));
        this->SetSpeedYRange(std::make_pair(5.f,10.f));
        this->SetScaleXRange(std::make_pair(0.05f,0.2f));
        this->SetRotationRange(std::make_pair(0.f,90.f));
			}

			inline ~SnowEmitter() {
			}

      inline void UpdateParticle(entt::registry &registry, entt::registry::entity_type particle) override {
        auto particle_par = registry.get<components::ParticleComponent>(particle);

        if (registry.has<components::PositionComponent>(particle) && particle_par.Life % 1 == 0) {
          auto particle_pos = registry.get<components::PositionComponent>(particle);
          particle_pos.x += particle_par.SpeedX;
          particle_pos.y += particle_par.SpeedY;
        }
        ++particle_par.Life;
      }

			inline entt::registry::entity_type Emit(entt::registry &registry, entt::registry::entity_type emitter) override {
        auto pos = registry.get<components::PositionComponent>(emitter);
        auto dim = registry.get<components::DimensionComponent>(emitter);

        auto particle = registry.create();
        
        unsigned int max_life = this->GetRandomValue<unsigned int>(this->Data.MaxLifeRange);
        int px = pos.x + dim.w*((int)this->GetRandomValue<int>(this->Data.PositionXRange)%(int)dim.w);
        double speedx = this->GetRandomValue<float>(this->Data.SpeedXRange, true);
        double speedy = this->GetRandomValue<float>(this->Data.SpeedYRange, true);
        float rotation = this->GetRandomValue<float>(this->Data.RotationRange);
        double scale = this->GetRandomValue<float>(this->Data.ScaleXRange, true);

        registry.assign<components::PositionComponent>(particle, px, pos.y, pos.z);
        registry.assign<components::DimensionComponent>(particle, dim.w, dim.h);
        registry.assign<components::ParticleComponent>(particle, max_life, 16.f, speedx, speedy, 1);
        registry.assign<components::SpriteComponent>(particle, this->TextureName, "particles");
        registry.assign<components::TransformComponent>(particle, rotation, glm::vec2(scale, scale));
        registry.assign<components::MovementComponent>(particle, glm::vec2(0.f, 100.f*speedy), false, true);
        //particle.assign<components::Body2DComponent>();
        return particle;
			}
	};
}}


