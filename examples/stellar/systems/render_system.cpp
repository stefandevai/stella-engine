#include "render_system.h"

#include <tuple>
#include <ctime>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../components/spatial_component.h"
#include "../components/particle_component.h"
#include "../components/animation_component.h"
#include "../components/transform_component.h"

RenderSystem::RenderSystem(int width, int height, std::unordered_map<std::string, stella::graphics::Texture*> &textures, stella::graphics::Display &display) : Textures(textures) {
	// Initialize shader and textures IDs
  GLint tex_ids[21] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
  this->Shader = new stella::graphics::Shader("assets/shaders/basic_shader.vsh", "assets/shaders/basic_shader.fsh");
  this->Shader->Enable();
  this->Shader->SetIntv("textures", tex_ids, 21);
  this->Shader->Disable();

	// Initialize Layer
	glm::mat4 proj = glm::ortho(0.0f, (float)width, (float)height, 0.0f, -1.0f, 1.0f);
  this->TileLayer = new SceneLayer(this->Shader, proj);

  // Initialize fire layer
	this->ParticleLayer = new FireLayer(this->Shader, proj, display);

  // Seed pseudo random number generator
	std::srand (static_cast <unsigned> (std::time(0)));
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
            spr.Sprite = new stella::graphics::Sprite(spa.x, spa.y,*tex);
          }
          else {
					  spr.Sprite = new stella::graphics::Sprite(spa.x, spa.y, spr.FrameDimensions.x, spr.FrameDimensions.y, *tex, 0);
          }

          // If the texture has a diferent resolution than the actual size we want
          if ((int)spr.Sprite->Dimensions.x != spa.w || (int)spr.Sprite->Dimensions.y != spa.h) {
            if (entity.has_component<TransformComponent>()) {
              auto trans = entity.component<TransformComponent>();
              spr.Sprite->SetDirectScale(glm::vec2((float)spa.w*trans->Scale.x, (float)spa.h*trans->Scale.y));
            }
            else spr.Sprite->SetDirectScale(glm::vec2((float)spa.w, (float)spa.h));
          }

					spr.Initialized = true;
				}
        if (entity.has_component<ParticleComponent>()) {
          this->ParticleLayer->Add(spr.Sprite);
        }
        else {
					this->TileLayer->Add(spr.Sprite);
        }
			}
			spr.InLayer = true;
		}

		//spr.Sprite->Dimensions.x = spa.w;
		//spr.Sprite->Dimensions.y = spa.h;
		spr.Sprite->Pos.x = spa.x;
		spr.Sprite->Pos.y = spa.y;
  });
 
	this->TileLayer->Render();
  //this->ParticleLayer->Render();
  this->ParticleLayer->RenderWithFBOs();
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
    if (ent.has_component<ParticleComponent>()) {
      this->ParticleLayer->Remove(spr->Sprite);
    }
    else {
			this->TileLayer->Remove(spr->Sprite);
    }
    spr->InLayer = false;
  }
}

