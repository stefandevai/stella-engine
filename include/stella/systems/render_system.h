#pragma once

#include "./system.h"
#include "../components.h"
#include "stella/graphics/scenelayer.h"
#include "stella/graphics/firelayer.h"
#include "stella/graphics/layers/basic_layer.h"

namespace stella
{
namespace systems
{
class RenderSystem : public System
{
  public:
    RenderSystem(entt::registry &registry, std::unordered_map<std::string, graphics::Texture*> &textures, graphics::Display& display)
      : m_textures(textures), m_display(display)
    {
      // TODO: Post an issue about not being able to add the listener
      //registry.on_destroy<components::SpriteComponent>().connect<&RenderSystem::remove_sprite_from_layer>(this);
      std::srand (static_cast <unsigned> (std::time(0)));
    }

    ~RenderSystem() override { }

    void update(entt::registry &registry, const double dt) override
    {
      registry.view<components::LayerComponent>().each([this](auto entity, auto &layer)
      {
        if (!layer.Initialized) {
          if (layer.ShaderId == "bloom")
          {
            m_layers[layer.Id] = std::shared_ptr<graphics::FireLayer>(new graphics::FireLayer(this->m_display));
          }
          else if (layer.ShaderId == "ui")
          {
            m_layers[layer.Id] = std::shared_ptr<graphics::BasicLayer>(new graphics::BasicLayer(this->m_display.GetWidth(), this->m_display.GetHeight(), layer.Fixed));
          }
          else
          {
            m_layers[layer.Id] = std::shared_ptr<graphics::BasicLayer>(new graphics::BasicLayer(this->m_display.GetWidth(), this->m_display.GetHeight(), layer.Fixed));
          }
          m_ordered_layers[layer.Order] = layer.Id;
          layer.Initialized = true;
        }
        // Update layer order if it has changed
        //else if (m_ordered_layers[layer.Id] != layer.Order) {
          //m_ordered_layers[layer.Id] = layer.Order;
        //}
      });

      registry.group<components::SpriteComponent>(entt::get<components::PositionComponent, components::DimensionComponent>).each([this, &registry](auto entity, auto &sprite, auto &pos, auto &dim)
      {
        // Adds sprite to layer
        if (!sprite.InLayer) {
          auto texdata = this->m_textures.find(sprite.TexName);
          if (texdata == this->m_textures.end()) {
            std::cout << "It was not possible to find " << sprite.TexName << " in loaded textures." << std::endl;
          }
          else {
            // Creates sprite if it doesn't exist yet
            if (!sprite.Initialized) {
              auto tex = texdata->second;
              // If no frame dimensions were provided
              if (sprite.FrameDimensions.x == 0) {
                sprite.Sprite = std::shared_ptr<graphics::Sprite>(new graphics::Sprite(glm::vec3(pos.x, pos.y, pos.z), *tex, sprite.Frame));
              }
              else {
                sprite.Sprite = std::shared_ptr<graphics::Sprite>(new graphics::Sprite(glm::vec3(pos.x, pos.y, pos.z), glm::vec2(sprite.FrameDimensions.x, sprite.FrameDimensions.y), *tex, sprite.Frame));
              }

              // If the texture has a diferent resolution than the actual size we want
              if ((int)sprite.Sprite->Dimensions.x != dim.w || (int)sprite.Sprite->Dimensions.y != dim.h) {
                if (registry.has<components::TransformComponent>(entity)) {
                  const auto &trans = registry.get<components::TransformComponent>(entity);
                  sprite.Sprite->SetDirectScale(glm::vec2((float)dim.w*trans.Scale.x, (float)dim.h*trans.Scale.y));
                }
                else sprite.Sprite->SetDirectScale(glm::vec2((float)dim.w, (float)dim.h));
              }

              sprite.Initialized = true;
            }
            m_layers[sprite.LayerId]->Add(sprite.Sprite);
          }
          sprite.InLayer = true;
        }

        //sprite.Sprite->Dimensions.x = dim.w;
        //sprite.Sprite->Dimensions.y = dim.h;
        // TODO: Dont't update static sprite position
        sprite.Sprite->Pos.x = (int)pos.x;
        sprite.Sprite->Pos.y = (int)pos.y;

      });
      
      for (auto const& order : m_ordered_layers) {
        if (!m_layers[order.second]->Fixed)
        {
            registry.group<components::CameraComponent>(entt::get<components::PositionComponent>).each([this, &order](auto entity, auto &camera, auto &pos)
            {
              m_layers[order.second]->SetViewMatrix(glm::lookAt(glm::vec3(pos.x, pos.y, pos.z), glm::vec3(pos.x, pos.y, pos.z - 1.f), glm::vec3(0.f, 1.f, 0.f)));
            });
        }
        m_layers[order.second]->Render();
      }
    }

  private:
    std::unordered_map<std::string, stella::graphics::Texture*> &m_textures;
    graphics::Display &m_display;
    std::unordered_map<std::string, std::shared_ptr<graphics::Layer>> m_layers;
    std::map<int, std::string> m_ordered_layers;

    RenderSystem() = delete;

    void remove_sprite_from_layer(entt::registry &registry, entt::entity entity, components::SpriteComponent &sprite)
    {
      if (sprite.InLayer)
      {
        m_layers[sprite.LayerId]->Remove(sprite.Sprite);
        sprite.InLayer = false;
      }
    }
};
} // namespace systems
} // namespace stella
