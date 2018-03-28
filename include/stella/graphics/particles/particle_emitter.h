#pragma once

#include <vector>
#include <entityx/entityx.h>

namespace stella {
namespace graphics {
	struct Particle {
		double x, y, w, h, spx, spy;
		int life;
	};

	class ParticleEmitter {
		public:
			inline virtual ~ParticleEmitter() {};
			virtual void UpdateParticle(stella::graphics::Particle &particle) = 0;
			virtual void GenerateParticle() = 0;

		protected:
			int PosX, PosY;
      std::vector<entityx::Entity> Particles;
			inline ParticleEmitter(int posx, int posy) : PosX(posx), PosY(posy) {}
	};
}}

