#include "render_system.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../components/spatial_component.h"
#include "../components/sprite_component.h"
#include "../components/sprite2_component.h"

RenderSystem::RenderSystem(int width, int height, std::unordered_map<std::string, stella::graphics::Texture*> &textures) : Textures(textures) {
	// Initialize shader and textures IDs
  GLint tex_ids[21] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
  this->Shader = new stella::graphics::Shader("assets/shaders/basic_shader.vsh", "assets/shaders/basic_shader.fsh");
  this->Shader->Enable();
  this->Shader->SetIntv("textures", tex_ids, 21);
  this->Shader->Disable();

	// Initialize Layer
  glm::mat4 proj = glm::ortho(0.0f, (float)width, (float)height, 0.0f, -1.0f, 1.0f);
  this->TileLayer = new SceneLayer(this->Shader, proj);
}

RenderSystem::~RenderSystem() { delete this->TileLayer; delete this->Shader; }

void RenderSystem::update(entityx::EntityManager &es,
                          entityx::EventManager &events,
                          entityx::TimeDelta dt) {


  es.each<SpatialComponent, TextureComponent>([this](entityx::Entity entity,
                                                     SpatialComponent &spa,
                                                     TextureComponent &tex) {
    tex.sprite->Pos.x = spa.x;
    tex.sprite->Pos.y = spa.y;

    if (!tex.InLayer) {
      this->TileLayer->Add(tex.sprite);
      tex.InLayer = true;
    }
  });

  es.each<SpatialComponent, SpriteComponent>([this](entityx::Entity entity,
                                                     SpatialComponent &spa,
                                                     SpriteComponent &spr) {
			if (!spr.InLayer) {
				auto tex = this->Textures.find(spr.TexName);
				
				if (tex == this->Textures.end()) {
					std::cout << "It was not possible to find " << spr.TexName << " in loaded textures." << std::endl;
				}
				else {
					spr.Sprite = new stella::graphics::Sprite(0, 0, 32, 32, *tex->second, 0);
					this->TileLayer->Add(spr.Sprite);
				}

				spr.InLayer = true;
			}
  });
  this->TileLayer->Render();
};

void RenderSystem::configure(entityx::EventManager &event_manager) {
  event_manager.subscribe<entityx::ComponentRemovedEvent<TextureComponent>>(
      *this);
}

void RenderSystem::receive(
    const entityx::ComponentRemovedEvent<TextureComponent> &ev) {
  auto ent = ev.entity;
  auto tex = ent.component<TextureComponent>();
  if (tex->InLayer) {
    this->TileLayer->Remove(tex->sprite);
    tex->InLayer = false;
  }
}

