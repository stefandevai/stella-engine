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

	class Emitter {
		public:
			inline virtual ~Emitter() {};
			virtual void UpdateParticle(entityx::Entity particle) = 0;
			virtual entityx::Entity Emit(entityx::Entity generator, entityx::EntityManager& es) = 0;

			inline const unsigned int& GetMaxParticles () const { return MaxParticles; }
			inline const bool& IsInitialized() const { return Initialized; }
			
			inline void SetMaxParticles (unsigned int new_max_particles) { MaxParticles = new_max_particles; }
			inline void Initialize() { Initialized = true; }

		protected:
			int PosX, PosY;
      unsigned int MaxParticles;
      std::string TextureName;
      bool Initialized = false;

			inline Emitter(int posx, int posy, unsigned int max_particles, std::string tex_name) : PosX(posx), PosY(posy), MaxParticles(max_particles), TextureName(tex_name) {}
	};
}}

