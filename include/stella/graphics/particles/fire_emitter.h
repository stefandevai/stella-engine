#include "stella/graphics/particles/emitter.h"

#include "../examples/stellar/components/particle_component.h"
#include "../examples/stellar/components/spatial_component.h"
#include "../examples/stellar/components/sprite_component.h"
#include "../examples/stellar/components/transform_component.h"

#include <iostream>
#include <cmath>

namespace stella {
namespace graphics {
	const double PI = 3.14159265;

	class FireEmitter : public Emitter {
		public:
			inline FireEmitter(int posx, int posy, unsigned int max_particles, std::string tex_name) : Emitter(posx, posy, max_particles, tex_name) {
			}

			inline ~FireEmitter() {
			}

			inline void UpdateParticle(entityx::Entity particle) {
        auto particle_spa = particle.component<SpatialComponent>();
        auto particle_trans = particle.component<TransformComponent>();
        auto particle_par = particle.component<ParticleComponent>();

        if ((int)particle_par->SpeedX % 3 == 0)
          particle_spa->x -= cos(particle_par->SpeedX*particle_par->Life*PI/180)*3 - 1;
        else if ((int)particle_par->SpeedY % 2 == 0)
          particle_spa->x += sin(particle_par->SpeedX*particle_par->Life*PI/180)*3;

        particle_spa->y += particle_par->SpeedY;


        if (particle_par->Life % 5 == 0) {
          particle_trans->Scale.x *= 0.8f;
          particle_trans->Scale.x = std::max(0.001f, particle_trans->Scale.x);
          particle_trans->Scale.y = particle_trans->Scale.x;
        }
        ++particle_par->Life;
			}

			inline entityx::Entity Emit(entityx::Entity generator, entityx::EntityManager& es) {
        auto spa = generator.component<SpatialComponent>();

        if (!this->Initialized) {
            auto particle = es.create();
            unsigned int MaxLife = std::rand() % 41 + 20;
            particle.assign<SpatialComponent>(spa->x, spa->y, spa->w, spa->h);
            particle.assign<ParticleComponent>(MaxLife, 1.0, 1.0, 1.0);
            particle.assign<TransformComponent>(0.f, glm::vec2(1.f, 1.f));
            return particle;
        }

        auto particle = es.create();
        unsigned int MaxLife = std::rand() % 41 + 20;
        double W = 10.0 + static_cast <float>(std::rand())/( static_cast <float> (RAND_MAX/(25.0f - 10.0f)));
        double SpeedX = 10.0 + static_cast <float> (std::rand()) /( static_cast <float> (RAND_MAX/(30.0f-10.0f)));
        double SpeedY = static_cast<float>(std::rand()) / (static_cast <float> (RAND_MAX/3.0));
        int px = spa->x + (-6 + std::rand()/(RAND_MAX/(6 + 6)));
        float rotation = static_cast <float> (std::rand()) /( static_cast <float> (RAND_MAX/(360.f)));

        particle.assign<SpatialComponent>(W, W, px, spa->y);
        particle.assign<ParticleComponent>(MaxLife, W, SpeedX, -SpeedY);
        particle.assign<SpriteComponent>(this->TextureName);
        particle.assign<TransformComponent>(rotation, glm::vec2(1.f, 1.f));
        return particle;
			}
	};
}}

