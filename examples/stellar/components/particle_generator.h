#include <vector>
#include <string>

#include <entityx/entityx.h>

struct ParticleGenerator {
	ParticleGenerator() {
		MaxParticles = 50;
		TextureName = "fire-particle";
		NumParticles = 0;
		Initialized = false;
	}

	std::string TextureName;
	unsigned int MaxParticles, NumParticles;
	std::vector<entityx::Entity> Particles;
	bool Initialized;
};

