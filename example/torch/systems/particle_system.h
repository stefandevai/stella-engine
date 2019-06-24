#pragma once

#include <entityx/entityx.h>

namespace ex = entityx;

class ParticleSystem : public ex::System<ParticleSystem> {
	public:
		ParticleSystem();
		~ParticleSystem();
		void update(ex::EntityManager &es, ex::EventManager &events,
              ex::TimeDelta dt) override;
  private:
    int Timer = 0, Velocity = 1;
};
	
