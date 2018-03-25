#include <vector>
#include <string>

#include <entityx/entityx.h>

struct ParticleGenerator {
	ParticleGenerator(std::string texture_name, unsigned int max_particles) : TextureName(texture_name), MaxParticles(mas_particles) {
		//MaxParticles = 60;
		//TextureName = "fire-particle";
		NumParticles = 0;
		Initialized = false;
	}

	std::string TextureName;
	unsigned int MaxParticles, NumParticles;
	std::vector<entityx::Entity> Particles;
	bool Initialized;
};

