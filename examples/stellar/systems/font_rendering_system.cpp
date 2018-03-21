#include "font_rendering_system.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../components/spatial_component.h"
#include "../components/text_component.h"

FontRenderingSystem::FontRenderingSystem(int width, int height, std::unordered_map<std::string, stella::graphics::Texture*> &fonts) : Fonts(fonts) {
  GLint tex_ids[21] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};

  this->Shader = new stella::graphics::Shader("assets/shaders/gui_shader.vsh", "assets/shaders/gui_shader.fsh");
  this->Shader->Enable();
	this->Shader->SetIntv("textures", tex_ids, 21);
  this->Shader->Disable();

	// Initialize Layer
  glm::mat4 proj = glm::ortho(0.0f, (float)width, (float)height, 0.0f, -1.0f, 1.0f);
  this->TextLayer = new SceneLayer(this->Shader, proj);
}

FontRenderingSystem::~FontRenderingSystem() {
	delete this->TextLayer;
	delete this->Shader;
}

void FontRenderingSystem::update(entityx::EntityManager &es, entityx::EventManager &events,
		entityx::TimeDelta dt) {
	es.each<SpatialComponent, TextComponent>([this](entityx::Entity entity,
																										 SpatialComponent &spa,
																										 TextComponent &text) {
			if (!text.InLayer) {
				auto tex = this->Fonts.find(text.Name);
				if (tex == this->Fonts.end()) {
					std::cout << "It was not possible to find " << text.Name << " in loaded fonts." << std::endl;
				}
				else {
					auto spr = new stella::graphics::Sprite(0, 0, 577, 9, *tex->second, 0);
					this->TextLayer->Add(spr);
					std::cout << "hereee" << std::endl;
				}
				text.InLayer = true;
			}
	});

  this->TextLayer->Render();
}
	
