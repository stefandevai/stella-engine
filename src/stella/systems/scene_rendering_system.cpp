#include <tuple>
#include <ctime>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "stella/components.h"
#include "stella/systems/scene_rendering_system.h"

namespace stella {
namespace systems {
SceneRenderingSystem::SceneRenderingSystem(int width, int height, std::unordered_map<std::string, stella::graphics::Texture*> &textures, stella::graphics::Display &display) : Textures(textures) {
	// Initialize shader and textures IDs
  GLint tex_ids[21] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
  this->Shader = new stella::graphics::Shader("assets/shaders/sprite_batch.vert", "assets/shaders/sprite_batch.frag");
  this->Shader->Enable();
  this->Shader->SetIntv("textures", tex_ids, 21);
  this->Shader->Disable();

	// Initialize Layer
	glm::mat4 proj = glm::ortho(0.0f, (float)width, (float)height, 0.0f, -1.0f, 1.0f);
  this->TileLayer = new graphics::SceneLayer(this->Shader, proj);

  // Initialize fire layer
	this->ParticleLayer = new graphics::FireLayer(display);

  // Seed pseudo random number generator
	std::srand (static_cast <unsigned> (std::time(0)));
}

SceneRenderingSystem::~SceneRenderingSystem() { delete this->TileLayer; delete this->Shader; }

void SceneRenderingSystem::update(ex::EntityManager &es,
                          ex::EventManager &events,
                          ex::TimeDelta dt) {

  es.each<components::SpriteComponent, components::PositionComponent, components::DimensionComponent>([this](ex::Entity entity,
                                                     components::SpriteComponent &spr,
                                                     components::PositionComponent &pos,
                                                     components::DimensionComponent &dim) {
		// Adds sprite to layer
		if (!spr.InLayer) {
			auto texdata = this->Textures.find(spr.TexName);
			if (texdata == this->Textures.end()) {
				std::cout << "It was not possible to find " << spr.TexName << " in loaded textures." << std::endl;
			}
			else {
				// Creates sprite if it doesn't exist yet
				if (!spr.Initialized) {
          auto tex = texdata->second;
          // If no frame dimensions were provided
          if (spr.FrameDimensions.x == 0) {
            spr.Sprite = std::shared_ptr<graphics::Sprite>(new graphics::Sprite(pos.x, pos.y,*tex));
          }
          else {
					  spr.Sprite = std::shared_ptr<graphics::Sprite>(new graphics::Sprite(pos.x, pos.y, spr.FrameDimensions.x, spr.FrameDimensions.y, *tex, spr.Frame));
          }

          // If the texture has a diferent resolution than the actual size we want
          if ((int)spr.Sprite->Dimensions.x != dim.w || (int)spr.Sprite->Dimensions.y != dim.h) {
            if (entity.has_component<components::TransformComponent>()) {
              auto trans = entity.component<components::TransformComponent>();
              spr.Sprite->SetDirectScale(glm::vec2((float)dim.w*trans->Scale.x, (float)dim.h*trans->Scale.y));
            }
            else spr.Sprite->SetDirectScale(glm::vec2((float)dim.w, (float)dim.h));
          }

					spr.Initialized = true;
				}
        if (entity.has_component<components::ParticleComponent>()) {
          this->ParticleLayer->Add(spr.Sprite);
        }
        else {
          this->TileLayer->Add(spr.Sprite);
        }
			}
			spr.InLayer = true;
		}

		//spr.Sprite->Dimensions.x = dim.w;
		//spr.Sprite->Dimensions.y = dim.h;
		// TODO: Dont't update static sprite position
		spr.Sprite->Pos.x = (int)pos.x;
		spr.Sprite->Pos.y = (int)pos.y;
  });
 
	// TODO: Join postprocessing passes to highly improve performance
	this->TileLayer->Render();
  //this->ParticleLayer->RenderWithFBOs();
  //this->ParticleLayer->Render();
};

void SceneRenderingSystem::configure(ex::EventManager &event_manager) {
  event_manager.subscribe<ex::ComponentRemovedEvent<components::SpriteComponent>>(
      *this);
}

void SceneRenderingSystem::receive(
    const ex::ComponentRemovedEvent<components::SpriteComponent> &ev) {
  auto ent = ev.entity;
  auto spr = ent.component<components::SpriteComponent>();

	if (spr->InLayer) {
    if (ent.has_component<components::ParticleComponent>()) {
      this->ParticleLayer->Remove(spr->Sprite);
    }
    else {
			this->TileLayer->Remove(spr->Sprite);
    }
    spr->InLayer = false;
  }
}
} // namespace systems
} // namespace stella

