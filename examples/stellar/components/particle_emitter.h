#include <vector>
#include <string>

#include <stella/particles.h>
#include <entityx/entityx.h>

struct ParticleEmitter {
	public:
		inline ParticleEmitter(std::string texture_name, unsigned int type, unsigned int max_particles) : TextureName(texture_name), Type(type), MaxParticles(max_particles) {
			NumParticles = 0;
			Initialized = false;
			Emitter = new stella::graphics::FireEmitter(0, 0, texture_name);
		}
		inline ~ParticleEmitter() {
			delete Emitter;
		}

		std::string TextureName;
		unsigned int Type, MaxParticles, NumParticles;
		std::vector<entityx::Entity> Particles;
		bool Initialized;
		stella::graphics::ParticleEmitter *Emitter;
};

