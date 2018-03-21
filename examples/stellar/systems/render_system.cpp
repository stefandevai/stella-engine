#include "render_system.h"

#include <tuple>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../components/spatial_component.h"
#include "../components/animation_component.h"

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

  es.each<SpatialComponent, SpriteComponent>([this](entityx::Entity entity,
                                                     SpatialComponent &spa,
                                                     SpriteComponent &spr) {
      // Adds sprite to layer
			if (!spr.InLayer) {
				auto tex = this->Textures.find(spr.TexName);
				if (tex == this->Textures.end()) {
					std::cout << "It was not possible to find " << spr.TexName << " in loaded textures." << std::endl;
				}
				else {
					// Creates sprite if it doesn't exist yet
					if (!spr.Initialized) {
						spr.Sprite = new stella::graphics::Sprite(0, 0, spa.w, spa.h, *tex->second, 0);
						spr.Initialized = true;
					}
					this->TileLayer->Add(spr.Sprite);
				}
				spr.InLayer = true;
			}

			if (entity.has_component<AnimationsComponent>()) {
				auto animations = entity.component<AnimationsComponent>();
				if (!animations->Initialized) {
					for (auto& anim: animations->Frames) {
						spr.Sprite->Animations.Add(std::get<0>(anim), std::get<1>(anim), std::get<2>(anim));
						spr.Sprite->Animations.Play(std::get<0>(anim));
					}
					animations->Initialized = true;
				}

				spr.Sprite->Update();
			}

			spr.Sprite->Pos.x = spa.x;
			spr.Sprite->Pos.y = spa.y;
  });
  this->TileLayer->Render();
};

void RenderSystem::configure(entityx::EventManager &event_manager) {
  event_manager.subscribe<entityx::ComponentRemovedEvent<SpriteComponent>>(
      *this);
}

void RenderSystem::receive(
    const entityx::ComponentRemovedEvent<SpriteComponent> &ev) {
  auto ent = ev.entity;
  auto spr = ent.component<SpriteComponent>();
  if (spr->InLayer) {
    this->TileLayer->Remove(spr->Sprite);
    spr->InLayer = false;
  }
}

