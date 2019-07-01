#include <tuple>
#include <ctime>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "stella/components.h"
#include "stella/systems/rendering_system.h"
#include "stella/graphics/texture.h"

namespace stella {
namespace systems {
RenderingSystem::RenderingSystem(std::unordered_map<std::string, stella::graphics::Texture*> &textures, stella::graphics::Display &display) : Textures(textures), Display(display) {
  // Seed pseudo random number generator
	std::srand (static_cast <unsigned> (std::time(0)));
}

RenderingSystem::~RenderingSystem() { }

void RenderingSystem::update(ex::EntityManager &es,
                          ex::EventManager &events,
                          ex::TimeDelta dt) {
  es.each<components::LayerComponent>([this](ex::Entity entity, components::LayerComponent &layer) {
      if (!layer.Initialized) {
        if (layer.ShaderId == "bloom")
        {
          layers[layer.Id] = std::shared_ptr<graphics::FireLayer>(new graphics::FireLayer(this->Display));
        }
        else if (layer.ShaderId == "ui")
        {
          layers[layer.Id] = std::shared_ptr<graphics::BasicLayer>(new graphics::BasicLayer(this->Display.GetWidth(), this->Display.GetHeight(), layer.Fixed));
        }
        else
        {
          layers[layer.Id] = std::shared_ptr<graphics::BasicLayer>(new graphics::BasicLayer(this->Display.GetWidth(), this->Display.GetHeight(), layer.Fixed));
        }
        layer_order[layer.Order] = layer.Id;
        layer.Initialized = true;
      }
      // Update layer order if it has changed
      //else if (layer_order[layer.Id] != layer.Order) {
        //layer_order[layer.Id] = layer.Order;
      //}
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
            spr.Sprite = std::shared_ptr<graphics::Sprite>(new graphics::Sprite(glm::vec3(pos.x, pos.y, pos.z), *tex, spr.Frame));
          }
          else {
            spr.Sprite = std::shared_ptr<graphics::Sprite>(new graphics::Sprite(glm::vec3(pos.x, pos.y, pos.z), glm::vec2(spr.FrameDimensions.x, spr.FrameDimensions.y), *tex, spr.Frame));
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
      }
      spr.InLayer = true;
    }

    //spr.Sprite->Dimensions.x = dim.w;
    //spr.Sprite->Dimensions.y = dim.h;
    // TODO: Dont't update static sprite position
    spr.Sprite->Pos.x = (int)pos.x;
    spr.Sprite->Pos.y = (int)pos.y;
  });
	
  for (auto const& order : layer_order) {
    if (!layers[order.second]->Fixed)
    {
        es.each<components::CameraComponent, components::PositionComponent>([this, order](ex::Entity entity, components::CameraComponent &camera, components::PositionComponent &pos)
        {
          layers[order.second]->SetViewMatrix(glm::lookAt(glm::vec3(pos.x, pos.y, pos.z), glm::vec3(pos.x, pos.y, pos.z - 1.f), glm::vec3(0.f, 1.f, 0.f)));
        });
    }
    layers[order.second]->Render();
    //std::cout << order.first << '\n';
    //std::cout << order.second << '\n';
  }
  //layers["basic"]->Render();
  //layers["particles"]->Render();
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
    layers[spr->LayerId]->Remove(spr->Sprite);
    spr->InLayer = false;
  }
}
} // namespace systems
} // namespace stella


