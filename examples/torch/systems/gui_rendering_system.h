#pragma once

#include <string>
#include <unordered_map>

#include <entityx/entityx.h>

#include "scenelayer.h"

namespace ex = entityx;

class GuiRenderingSystem : public ex::System<GuiRenderingSystem> {
	public:
 		GuiRenderingSystem(int width, int height, std::unordered_map<std::string, stella::graphics::Texture*> &fonts);
		~GuiRenderingSystem();
		void update(ex::EntityManager &es, ex::EventManager &events,
								ex::TimeDelta dt) override;
	private:
		SceneLayer *TextLayer;
		stella::graphics::Shader *Shader;
		std::unordered_map<std::string, stella::graphics::Texture*> &Fonts;
};
	
