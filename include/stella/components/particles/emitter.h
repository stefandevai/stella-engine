#pragma once

#include <vector>
#include <string>
#include <entt/entity/registry.hpp>
#include "../particle_component.h"
#include "../position_component.h"
#include "../dimension_component.h"
#include "../sprite_component.h"
#include "../transform_component.h"

namespace stella {
namespace graphics {
  struct ParticleVariationData {
    std::pair<int, int> MaxLifeRange = std::make_pair(0,0);
    std::pair<int, int> PositionXRange = std::make_pair(0,0);
    std::pair<int, int> PositionYRange = std::make_pair(0,0);
    std::pair<float, float> SpeedXRange = std::make_pair(0.f,0.f);
    std::pair<float, float> SpeedYRange = std::make_pair(0.f,0.f);
    std::pair<float, float> RotationRange = std::make_pair(0.f,0.f);
    std::pair<float, float> ScaleXRange = std::make_pair(1.f,1.f);
    std::pair<float, float> ScaleYRange = std::make_pair(-1.f,-1.f);
  };

	class Emitter {
		public:
			inline virtual ~Emitter() {};
      virtual void UpdateParticle(entt::registry &registry, entt::registry::entity_type particle) {
        auto &particle_par = registry.get<components::ParticleComponent>(particle);

        if (registry.has<components::PositionComponent>(particle)) {
          auto &particle_pos = registry.get<components::PositionComponent>(particle);
          particle_pos.x += particle_par.SpeedX;
          particle_pos.y += particle_par.SpeedY;
        }

        ++particle_par.Life;
      }

			virtual entt::registry::entity_type Emit(entt::registry &registry, entt::registry::entity_type emitter) {
        auto &pos = registry.get<components::PositionComponent>(emitter);
        auto &dim = registry.get<components::DimensionComponent>(emitter);

        auto particle = registry.create();
        
        unsigned int max_life = this->GetRandomValue<unsigned int>(this->Data.MaxLifeRange);
        int px = pos.x + this->GetRandomValue<int>(this->Data.PositionXRange);
        int py = pos.y + this->GetRandomValue<int>(this->Data.PositionYRange);
        double speedx = this->GetRandomValue<float>(this->Data.SpeedXRange, true);
        double speedy = this->GetRandomValue<float>(this->Data.SpeedYRange, true);
        float rotation = this->GetRandomValue<float>(this->Data.RotationRange);
        double scalex = this->GetRandomValue<float>(this->Data.ScaleXRange, true);
        double scaley = this->GetRandomValue<float>(this->Data.ScaleYRange, true);

        // If we havent set a ScaleX range, we suppose we want proportional sprites in generation;
        // therefore we check if scaley is -1.f, as it is the default value
        if (scaley == -1.f) scaley = scalex;

        registry.assign<components::PositionComponent>(particle, px, py, pos.z);
        registry.assign<components::DimensionComponent>(particle, dim.w, dim.h);
        registry.assign<components::ParticleComponent>(particle, max_life, scalex, speedx, speedy);
        registry.assign<components::SpriteComponent>(particle, this->TextureName, "particles");
        registry.assign<components::TransformComponent>(particle, rotation, glm::vec2(scalex, scaley));
        registry.assign<components::MovementComponent>(particle, glm::vec2(speedx, speedy), false);
        //particle.assign<components::Body2DComponent>();
        return particle;
			}

			template<typename T>
			static T GetRandomValue(std::pair<T,T> range, bool exclude_zero = false) {
			  if (range.first == range.second) return range.first;
			  if (!exclude_zero)
          return range.first + static_cast<T>(std::rand()) / (static_cast<T>(RAND_MAX/(range.second - range.first)));
        else {
          T value = range.first + static_cast<T>(std::rand()) / (static_cast<T>(RAND_MAX/(range.second - range.first)));
          while(!value)
            value = range.first + static_cast<T>(std::rand()) / (static_cast<T>(RAND_MAX/(range.second - range.first)));
          return value;
        }
			}

			inline const unsigned int& GetMaxParticles () const { return MaxParticles; }
			inline const bool& IsInitialized() const { return Initialized; }
			
			inline void Initialize() { Initialized = true; }
			inline void SetMaxParticles (unsigned int new_max_particles) { MaxParticles = new_max_particles; }
      inline void SetMaxLifeRange(std::pair<int,int> range) { Data.MaxLifeRange = range; }
      inline void SetPositionXRange(std::pair<int,int> range) { Data.PositionXRange = range; }
      inline void SetPositionYRange(std::pair<int,int> range) { Data.PositionYRange = range; }
      inline void SetSpeedXRange(std::pair<float,float> range) { Data.SpeedXRange = range; }
      inline void SetSpeedYRange(std::pair<float,float> range) { Data.SpeedYRange = range; }
      inline void SetRotationRange(std::pair<float,float> range) { Data.RotationRange = range; }
      inline void SetScaleXRange(std::pair<float,float> range)  { Data.ScaleXRange = range; }
      inline void SetScaleYRange(std::pair<float,float> range) { Data.ScaleYRange = range; }

		protected:
			int PosX, PosY;
      unsigned int MaxParticles;
      std::string TextureName;
      bool Initialized = false;
      ParticleVariationData Data;

      inline Emitter(int posx, int posy, unsigned int max_particles, const std::string& tex_name) : PosX(posx), PosY(posy), MaxParticles(max_particles), TextureName(tex_name) {}
	};
}}

