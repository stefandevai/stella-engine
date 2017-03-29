#pragma once

#include <entityx/entityx.h>
#include <stella/stella.h>

class LightingSystem : public entityx::System<LightingSystem> {
	public:
		LightingSystem(stella::graphics::Shader *shad);
		~LightingSystem();
		void update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt) override;
	private:
		stella::graphics::Shader *shader;
		int light_counter;
};
