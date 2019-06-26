#include <tuple>
#include <ctime>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "stella/components/game_components.h"
#include "stella/systems/rendering_system.h"
#include "stella/graphics/texture.h"

namespace stella {
namespace systems {
RenderingSystem::RenderingSystem(std::unordered_map<std::string, stella::graphics::Texture*> &textures, stella::graphics::Display &display) : Textures(textures) {

	// Initialize shader and textures IDs
  GLint tex_ids[21] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
  this->Shader = new stella::graphics::Shader("assets/shaders/sprite_batch.vert", "assets/shaders/sprite_batch.frag");
  this->Shader->Enable();
  this->Shader->SetIntv("textures", tex_ids, 21);
  this->Shader->Disable();

	// Initialize Layer
  this->proj = glm::ortho(0.0f, (float)display.GetWidth(), (float)display.GetHeight(), 0.0f, -20.0f, 10.0f);
  this->TileLayer = new graphics::SceneLayer(this->Shader, proj);

  // Seed pseudo random number generator
	std::srand (static_cast <unsigned> (std::time(0)));
}

RenderingSystem::~RenderingSystem() { delete this->TileLayer; delete this->Shader; }

void RenderingSystem::update(ex::EntityManager &es,
                          ex::EventManager &events,
                          ex::TimeDelta dt) {
  es.each<components::LayerComponent>([this](ex::Entity entity,
                                                     components::LayerComponent &layer) {
      if (!layer.Initialized) {
        layers[layer.Id] = std::shared_ptr<graphics::BasicLayer>(new graphics::BasicLayer(this->Shader, this->proj));
        layer_order[layer.Id] = layer.Order;
        layer.Initialized = true;
      }
      // Update layer order if it has changed
      else if (layer_order[layer.Id] != layer.Order) {
        layer_order[layer.Id] = layer.Order;
      }
  });
 
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
            spr.Sprite = new stella::graphics::Sprite(glm::vec3(pos.x, pos.y, pos.z), *tex, spr.Frame);
          }
          else {
            spr.Sprite = new stella::graphics::Sprite(glm::vec3(pos.x, pos.y, pos.z), glm::vec2(spr.FrameDimensions.x, spr.FrameDimensions.y), *tex, spr.Frame);
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
        layers[spr.LayerId]->Add(spr.Sprite);
        //this->TileLayer->Add(spr.Sprite);
      }
      spr.InLayer = true;
    }

    //spr.Sprite->Dimensions.x = dim.w;
    //spr.Sprite->Dimensions.y = dim.h;
    // TODO: Dont't update static sprite position
    spr.Sprite->Pos.x = (int)pos.x;
    spr.Sprite->Pos.y = (int)pos.y;
  });
	
	//this->TileLayer->Render();
  for (auto const& order : layer_order) {
    layers[order.first]->Render();
  }

};

void RenderingSystem::configure(ex::EventManager &event_manager) {
  event_manager.subscribe<ex::ComponentRemovedEvent<components::SpriteComponent>>(
      *this);
}

void RenderingSystem::receive(
    const ex::ComponentRemovedEvent<components::SpriteComponent> &ev) {
  auto ent = ev.entity;
  auto spr = ent.component<components::SpriteComponent>();

	if (spr->InLayer) {
    this->TileLayer->Remove(spr->Sprite);
    spr->InLayer = false;
  }
}
} // namespace systems
} // namespace stella


