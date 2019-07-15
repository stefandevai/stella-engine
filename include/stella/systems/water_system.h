#pragma once

#include <ctime>
#include "../core/resource.h"
#include "./system.h"
//#include "../components/layer_component.h"
//#include "../components/sprite_component.h"
#include "../components/water_component.h"
#include "../components/position_component.h"
#include "../components/dimension_component.h"
//#include "../components/transform_component.h"
#include "../components/camera_component.h"
#include "stella/graphics/layers/water_layer.h"

namespace stella
{
namespace systems
{
class WaterSystem : public System
{
  private:
    const std::string DEFAULT_LAYER_NAME = "basic";
    graphics::Display &m_display;
    graphics::WaterLayer m_layer{m_display.GetWidth(), m_display.GetHeight(), true}

  public:
    WaterSystem(entt::registry &registry, graphics::Display& display)
      : m_display(display)
    {
      registry.on_destroy<components::WaterComponent>().connect<&WaterSystem::remove_water_from_layer>(this);
    }

    ~WaterSystem() override { }

    void update(entt::registry &registry, const double dt) override
    {
      registry.group<components::SpriteComponent>(entt::get<components::PositionComponent, components::DimensionComponent>).each([this, &registry](auto entity, auto &sprite, auto &pos, auto &dim)
      {
        // Adds sprite to layer
        if (!sprite.InLayer) {
          auto tex = m_textures.load(sprite.TexName);
          if (tex == nullptr) {
            std::cout << "It was not possible to find " << sprite.TexName << " in loaded textures." << std::endl;
          }
          //auto texdata = this->m_textures.find(sprite.TexName);
          //if (texdata == this->m_textures.end()) {
            //std::cout << "It was not possible to find " << sprite.TexName << " in loaded textures." << std::endl;
          //}
          else {
            // Creates sprite if it doesn't exist yet
            if (!sprite.Initialized) {
              // If no frame dimensions were provided
              if (sprite.FrameDimensions.x == 0) {
                sprite.Sprite = std::shared_ptr<graphics::Sprite>(new graphics::Sprite(glm::vec3(pos.x, pos.y, pos.z), *tex, sprite.Frame));
              }
              else {
                sprite.Sprite = std::shared_ptr<graphics::Sprite>(new graphics::Sprite(glm::vec3(pos.x, pos.y, pos.z), glm::vec2(sprite.FrameDimensions.x, sprite.FrameDimensions.y), *tex, sprite.Frame));
              }

              // If the texture has a diferent resolution than the actual size we want
              //if ((int)sprite.Sprite->Dimensions.x != dim.w || (int)sprite.Sprite->Dimensions.y != dim.h) {
                if (registry.has<components::TransformComponent>(entity)) {
                  const auto &trans = registry.get<components::TransformComponent>(entity);
                  sprite.Sprite->SetDirectScale(glm::vec2((float)dim.w*trans.Scale.x, (float)dim.h*trans.Scale.y));
                  sprite.Sprite->SetRotation(trans.Rotation);
                }
                else sprite.Sprite->SetDirectScale(glm::vec2((float)dim.w, (float)dim.h));
              //}

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

      //for (auto const& order : m_ordered_layers) {
        //if (!m_layers[order.second]->Fixed)
        //{
            //registry.group<components::CameraComponent>(entt::get<components::PositionComponent>).each([this, &order](auto entity, auto &camera, auto &pos)
            //{
              //m_layers[order.second]->SetViewMatrix(glm::lookAt(glm::vec3(pos.x, pos.y, pos.z), glm::vec3(pos.x, pos.y, pos.z - 1.f), glm::vec3(0.f, 1.f, 0.f)));
            //});
        //}
        m_layer.Render();
        //m_layers[order.second]->Render();
      //}
    }
  private:
    WaterSystem() = delete;

    void remove_water_from_layer(entt::registry &registry, entt::entity entity)
    {
      //auto& sprite = registry.get<components::SpriteComponent>(entity);
      //if (sprite.InLayer)
      //{
        //m_layers[sprite.LayerId]->Remove(sprite.Sprite);
        //sprite.InLayer = false;
      //}
    }
};
} // namespace systems
} // namespace stella