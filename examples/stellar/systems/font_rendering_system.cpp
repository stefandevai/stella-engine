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
	es.each<SpatialComponent, TextComponent>([this, &dt](entityx::Entity entity,
																										 SpatialComponent &spa,
																										 TextComponent &text) {
			if (!text.InLayer) {
				auto tex = this->Fonts.find(text.Name);
				if (tex == this->Fonts.end()) {
					std::cout << "It was not possible to find " << text.Name << " in loaded fonts." << std::endl;
				}
				else {
					text.Text = "FPS: 64.44";

					text.Spaces = 0;
					int stride = 0;

					for (auto c: text.Text) {
						int frame = (int)c - 33;
						if (frame == -1) {
							stride += 1;
							text.Spaces += 1;
						}
						else {
							auto spr = new stella::graphics::Sprite(spa.x + spa.w*stride, spa.y, spa.w, spa.h, *tex->second, frame);
							text.Sprites.push_back(spr);
							this->TextLayer->Add(spr);
							stride += 1;
						}
					}
				}
				text.InLayer = true;
			}
			
			if (!text.IsStatic) {
				text.Text = "FPS: ";
				text.Text += std::to_string(1.0f/dt);
				int stride = 0;

				if (text.Text.size() - text.Spaces != text.Sprites.size()) {
					auto spr = text.Sprites.begin();
					auto c = text.Text.begin();
					for (; c != text.Text.end(); ++c) {
						int frame = (int)*c - 33;
						if (frame == -1) stride += 1;
						else {
							(*spr)->SetDirectFrame(frame);	
							++spr;
							stride += 1;
						}
						if (spr == text.Sprites.end()) 
							break;
					}

					if (c != text.Text.end()) {
						auto tex = this->Fonts.find(text.Name);
						for (; c != text.Text.end(); ++c) {
							int frame = (int)*c - 33;
							if (frame == -1) {
								stride += 1;
								text.Spaces += 1;
							}
							else {
								auto spr = new stella::graphics::Sprite(spa.x + spa.w*stride, spa.y, spa.w, spa.h, *tex->second, frame);
								text.Sprites.push_back(spr);
								this->TextLayer->Add(spr);
								stride += 1;
							}
						}
					}

					else if (spr != text.Sprites.end()) {
						auto it_copy = spr;

						for (; spr != text.Sprites.end(); ++spr) {
							this->TextLayer->Remove(*spr);
						}

						text.Sprites.erase(it_copy, text.Sprites.end());

					}
				}
				else {
					auto spr = text.Sprites.begin();
					for (auto& c: text.Text) {
						int frame = (int)c - 33;
						if (frame == -1) stride += 1;
						else {
							(*spr)->SetDirectFrame(frame);	
							++spr;
						}
					}
				}
			}
	});

  this->TextLayer->Render();
}
	
