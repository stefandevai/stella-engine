#include "gui_rendering_system.h"
#include <stella/stella.h>

#include <cmath>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../components/text_component.h"
#include "../components/position_component.h"
#include "../components/dimension_component.h"

GuiRenderingSystem::GuiRenderingSystem(int width, int height, std::unordered_map<std::string, stella::graphics::Texture*> &fonts) : Fonts(fonts) {
  GLint tex_ids[21] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};

  this->Shader = new stella::graphics::Shader("assets/shaders/gui.vert", "assets/shaders/gui.frag");
  this->Shader->Enable();
	this->Shader->SetIntv("textures", tex_ids, 21);
  this->Shader->Disable();

	// Initialize Layer
  glm::mat4 proj = glm::ortho(0.0f, (float)width, (float)height, 0.0f, -1.0f, 1.0f);
  this->TextLayer = new SceneLayer(this->Shader, proj);
}

GuiRenderingSystem::~GuiRenderingSystem() {
	delete this->TextLayer;
	delete this->Shader;
}

void GuiRenderingSystem::update(ex::EntityManager &es, ex::EventManager &events,
		ex::TimeDelta dt) {
	es.each<TextComponent, PositionComponent, DimensionComponent>([this, &dt](ex::Entity entity,
                                                       TextComponent &text,
                                                       PositionComponent &pos,
																										   DimensionComponent &dim) {
			if (!text.InLayer) {
				auto tex = this->Fonts.find(text.Name);
				if (tex == this->Fonts.end()) {
					std::cout << "It was not possible to find " << text.Name << " in loaded fonts." << std::endl;
				}
				else {
					text.Spaces = 0;
					int stride = 0;

					for (auto c: text.Text) {
						int frame = (int)c - 33;
						if (frame == -1) {
							stride += 1;
							text.Spaces += 1;
						}
						else {
							auto spr = new stella::graphics::Sprite(pos.x + dim.w*stride, pos.y, dim.w, dim.h, *tex->second, frame);
							text.Sprites.push_back(spr);
							this->TextLayer->Add(spr);
							stride += 1;
						}
					}
				}
				text.InLayer = true;
			}
			
			if (!text.IsStatic) {
				int stride = 0;
				auto spr = text.Sprites.begin();
				auto c = text.Text.begin();
				text.Spaces = 0;

				for (; c != text.Text.end(); ++c) {
					if (spr == text.Sprites.end()) 
						break;
					int frame = (int)*c - 33;
					if (frame == -1) {
						stride += 1;
						text.Spaces += 1;
					}
					else {
						(*spr)->Pos.x = pos.x + dim.w*stride;
						(*spr)->SetDirectFrame(frame);	
						++spr;
						stride += 1;
					}
				}

				// If the text is bigger than the last printed
				if (c != text.Text.end()) {
					auto tex = this->Fonts.find(text.Name);
					for (; c != text.Text.end(); ++c) {
						int frame = (int)*c - 33;
						if (frame == -1) {
							stride += 1;
							text.Spaces += 1;
						}
						else {
							auto spr = new stella::graphics::Sprite(pos.x + dim.w*stride, pos.y, dim.w, dim.h, *tex->second, frame);
							text.Sprites.push_back(spr);
							this->TextLayer->Add(spr);
							stride += 1;
						}
					}
				}

				// If the text is smaller than the last printed
				else if (spr != text.Sprites.end()) {
					auto iterator_copy = spr;
					for (; spr != text.Sprites.end(); ++spr) this->TextLayer->Remove(*spr);

					text.Sprites.erase(iterator_copy, text.Sprites.end());
				}
			}
	});

  this->TextLayer->Render();
}
	
