#pragma once

#include <vector>
#include <string>
#include <entityx/entityx.h>

namespace stella {
namespace graphics {
  struct Particle {
    double x, y, w, h, spx, spy;
    int life;
  };

	class ParticleEmitter {
		public:
      std::vector<entityx::Entity> Particles;

			inline virtual ~ParticleEmitter() {};
			virtual void UpdateParticle(entityx::Entity particle) = 0;
			virtual entityx::Entity GenerateParticle(entityx::Entity generator, entityx::EntityManager& es) = 0;

		protected:
			int PosX, PosY;
      std::string TextureName;
			inline ParticleEmitter(int posx, int posy, std::string tex_name) : PosX(posx), PosY(posy), TextureName(tex_name) {}
	};
}}

