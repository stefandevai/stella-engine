#pragma once

#include <entityx/entityx.h>

class AnimationSystem : public entityx::System<AnimationSystem> {
	public:
		AnimationSystem();
		~AnimationSystem();
		void update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt) override;
	private:
};
