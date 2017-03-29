#pragma once

#include <entityx/entityx.h>
#include <stella/stella.h>

#include "scenelayer.h"

class RenderSystem : public entityx::System<RenderSystem> {
	public:
		SceneLayer*	TileLayer;
		stella::graphics::Shader *shader;

		RenderSystem(int width, int height, stella::graphics::Shader *shad);
		~RenderSystem();
		void update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt) override;
};

