#pragma once

#include <string>
#include <unordered_map>

#include <entityx/entityx.h>
#include <stella/stella.h>

#include "scenelayer.h"

class FontRenderingSystem : public entityx::System<FontRenderingSystem> {
	public:
 		FontRenderingSystem(int width, int height, std::unordered_map<std::string, stella::graphics::Texture*> &fonts);
		~FontRenderingSystem();
		void update(entityx::EntityManager &es, entityx::EventManager &events,
								entityx::TimeDelta dt) override;
	private:
		SceneLayer *TextLayer;
		stella::graphics::Shader *Shader;
		std::unordered_map<std::string, stella::graphics::Texture*> &Fonts;
};
	
