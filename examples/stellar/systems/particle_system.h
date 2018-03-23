#pragma once

#include <entityx/entityx.h>

class ParticleSystem : public entityx::System<ParticleSystem> {
	public:
		ParticleSystem();
		~ParticleSystem();
		void update(entityx::EntityManager &es, entityx::EventManager &events,
              entityx::TimeDelta dt) override;

	private:
		std::vector<entityx::Entity> ParticlesAlive, ParticlesDead;

		void CreateParticle(entityx::EntityManager& es); 
};
	
