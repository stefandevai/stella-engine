#include "stella/graphics/particles/emitter.h"

#include "../examples/stellar/components/particle_component.h"
#include "../examples/stellar/components/spatial_component.h"
#include "../examples/stellar/components/sprite_component.h"

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
        auto par = particle.component<ParticleComponent>();

        if ((int)par->SpeedX % 3 == 0)
          particle_spa->x -= cos(par->SpeedX*par->Life*PI/180)*3 - 1;
        else if ((int)par->SpeedY % 2 == 0)
          particle_spa->x += sin(par->SpeedX*par->Life*PI/180)*3;

        particle_spa->y += par->SpeedY;


        if (par->Life % 5 == 0) {
          particle_spa->w *= 0.80;
          particle_spa->w = std::max(1.0f, (float)particle_spa->w);
          particle_spa->h = particle_spa->w;
        }
        ++par->Life;
			}

			inline entityx::Entity Emit(entityx::Entity generator, entityx::EntityManager& es) {
        auto spa = generator.component<SpatialComponent>();

        if (!this->Initialized) {
            auto particle = es.create();
            unsigned int MaxLife = std::rand() % 41 + 20;
            particle.assign<SpatialComponent>(spa->x, spa->y, spa->w, spa->h);
            particle.assign<ParticleComponent>(MaxLife, 1.0, 1.0, 1.0);
            return particle;
        }

        auto particle = es.create();
        unsigned int MaxLife = std::rand() % 41 + 20;
        double W = 10.0 + static_cast <float>(std::rand())/( static_cast <float> (RAND_MAX/(25.0f - 10.0f)));
        double SpeedX = 10.0 + static_cast <float> (std::rand()) /( static_cast <float> (RAND_MAX/(30.0f-10.0f)));
        double SpeedY = static_cast<float>(std::rand()) / (static_cast <float> (RAND_MAX/3.0));
        int px = spa->x + (-6 + std::rand()/(RAND_MAX/(6 + 6)));

        particle.assign<SpatialComponent>(W, W, px, spa->y);
        particle.assign<ParticleComponent>(MaxLife, W, SpeedX, -SpeedY);
        particle.assign<SpriteComponent>(this->TextureName);
        return particle;
			}
	};
}}
