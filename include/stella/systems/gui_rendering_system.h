#pragma once

#include <string>
#include <unordered_map>
#include <entityx/entityx.h>

#include "stella/graphics/scenelayer.h"

namespace ex = entityx;

namespace stella {
namespace systems {
class GuiRenderingSystem : public ex::System<GuiRenderingSystem> {
	public:
 		GuiRenderingSystem(int width, int height, std::unordered_map<std::string, graphics::Texture*> &fonts);
		~GuiRenderingSystem();
		void update(ex::EntityManager &es, ex::EventManager &events,
								ex::TimeDelta dt) override;
	private:
    graphics::SceneLayer *TextLayer;
		graphics::Shader *Shader;
		std::unordered_map<std::string, stella::graphics::Texture*> &Fonts;
};
} // namespace systems
} // namespace stella
	
