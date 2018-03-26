#include "stella/graphics/particles/particle_emitter.h"

#include <iostream>
#include <cmath>

namespace stella {
namespace graphics {
	const double PI = 3.14159265;

	class FireEmitter : public ParticleEmitter {
		public:
			inline FireEmitter(int posx, int posy) : ParticleEmitter(posx, posy) {
			}

			inline ~FireEmitter() {
			}

			inline void UpdateParticle(stella::graphics::Particle &particle) {
				if ((int)particle.spx % 3 == 0)
					particle.x -= cos(particle.spx*particle.life*PI/180)*3 - 1;
				else if ((int)particle.spy % 2 == 0)
					particle.x += sin(particle.spx * particle.life * PI/180)*3;

				particle.y += particle.spy;

				if (particle.life % 5 == 0) {
					particle.w *= 0.80;
					particle.w = std::max(1.0f, (float)particle.w);
					particle.h = particle.w;
				}
				++particle.life;
			}

			inline void GenerateParticle() {
			}
	};
}}

