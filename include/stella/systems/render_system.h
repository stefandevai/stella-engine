#pragma once

#include "stella/components/camera.h"
#include "stella/components/dimension.h"
#include "stella/components/layer.h"
#include "stella/components/position.h"
#include "stella/components/sprite_component.h"
#include "stella/components/transform_component.h"
#include "stella/core/resource.h"
#include "stella/graphics/layers/basic_layer.h"
#include "stella/graphics/layers/firelayer.h"
#include "stella/systems/system.h"
#include <ctime>

namespace stella
{
namespace systems
{
  class RenderSystem : public System
  {
  private:
    const std::string DEFAULT_LAYER_NAME = "basic";
    core::ResourceManager<graphics::Texture, const std::string>& m_textures;
    graphics::Display& m_display;
    std::unordered_map<std::string, std::shared_ptr<graphics::Layer>> m_layers{
        {DEFAULT_LAYER_NAME,
         std::make_shared<graphics::BasicLayer> (m_display.GetWidth(),
                                                 m_display.GetHeight(),
                                                 "assets/shaders/sprite_batch.vert",
                                                 "assets/shaders/sprite_batch.frag",
                                                 false)}};
    std::map<int, std::string> m_ordered_layers{{256, DEFAULT_LAYER_NAME}};

  public:
    RenderSystem (entt::registry& registry,
                  core::ResourceManager<graphics::Texture, const std::string>& textures,
                  graphics::Display& display)
      : m_textures (textures), m_display (display)
    {
      registry.on_construct<components::Layer>().connect<&RenderSystem::initialize_layer> (this);
      registry.on_destroy<components::SpriteComponent>().connect<&RenderSystem::remove_sprite_from_layer> (this);
      std::srand (static_cast<unsigned> (std::time (nullptr)));
    }

    ~RenderSystem() override {}

    void update (entt::registry& registry, const double dt) override
    {
      registry
          .group<components::SpriteComponent> (entt::get<components::Position, components::Dimension>)
          .each ([this, &registry] (auto entity, auto& sprite, auto& pos, auto& dim) {
            // Adds sprite to layer
            if (!sprite.InLayer && sprite.Initialized)
            {
              m_layers[sprite.LayerId]->Add (sprite.Sprite);
              sprite.InLayer = true;
            }
            else if (!sprite.InLayer)
            {
              auto tex = m_textures.load (sprite.TexName);
              if (tex == nullptr)
              {
                std::cout << "It was not possible to find " << sprite.TexName << " in loaded textures." << std::endl;
              }
              else
              {
                // Creates sprite if it doesn't exist yet
                if (!sprite.Initialized)
                {
                  // If no frame dimensions were provided
                  if (sprite.FrameDimensions.x == 0)
                  {
                    sprite.Sprite = std::shared_ptr<graphics::Sprite> (
                        new graphics::Sprite (glm::vec3 (pos.x, pos.y, pos.z), *tex, sprite.Frame));
                  }
                  else
                  {
                    sprite.Sprite = std::shared_ptr<graphics::Sprite> (
                        new graphics::Sprite (glm::vec3 (pos.x, pos.y, pos.z),
                                              glm::vec2 (sprite.FrameDimensions.x, sprite.FrameDimensions.y),
                                              *tex,
                                              sprite.Frame));
                  }
                  if (registry.has<components::TransformComponent> (entity))
                  {
                    const auto& trans = registry.get<components::TransformComponent> (entity);
                    sprite.Sprite->SetDirectScale (
                        glm::vec2 ((float) dim.w * trans.Scale.x, (float) dim.h * trans.Scale.y));
                    sprite.Sprite->SetRotation (trans.Rotation);
                  }
                  else
                  {
                    sprite.Sprite->SetDirectScale (glm::vec2 ((float) dim.w, (float) dim.h));
                  }
                  sprite.Initialized = true;
                }
              }
            }
            // Sprite inLayer and position changed
            if (pos.has_changed())
            {
              m_layers[sprite.LayerId]->Remove (sprite.Sprite);
              sprite.InLayer = false;

              sprite.Sprite->Pos.x = (int) pos.x;
              sprite.Sprite->Pos.y = (int) pos.y;
              sprite.Sprite->Pos.z = (int) pos.z;
            }

            if (dim.has_changed())
            {
              m_layers[sprite.LayerId]->Remove (sprite.Sprite);
              sprite.InLayer = false;

              sprite.Sprite->Dimensions.x = dim.w;
              sprite.Sprite->Dimensions.y = dim.h;
            }
            
            pos.last_x = pos.x;
            pos.last_y = pos.y;
            pos.last_z = pos.z;
            dim.last_w = dim.w;
            dim.last_h = dim.h;

            if (!sprite.InLayer)
            {
              m_layers[sprite.LayerId]->Add (sprite.Sprite);
              sprite.InLayer = true;
            }
          });

      const auto camera_entity = *registry.view<stella::components::Camera>().begin();
      auto& camera_pos         = registry.get<components::Position> (camera_entity);

      for (auto const& order : m_ordered_layers)
      {
        if (!m_layers[order.second]->Fixed)
        {
          m_layers[order.second]->SetViewMatrix (
              glm::lookAt (glm::vec3 (camera_pos.x, camera_pos.y, camera_pos.z),
                           glm::vec3 (camera_pos.x, camera_pos.y, camera_pos.z - 1.f),
                           glm::vec3 (0.f, 1.f, 0.f)));
        }
        m_layers[order.second]->Render();
      }
    }

  private:
    RenderSystem() = delete;

    void remove_sprite_from_layer (entt::registry& registry, entt::entity entity)
    {
      auto& sprite = registry.get<components::SpriteComponent> (entity);
      if (sprite.InLayer)
      {
        m_layers[sprite.LayerId]->Remove (sprite.Sprite);
        sprite.InLayer = false;
      }
    }

    void initialize_layer (entt::registry& registry, entt::entity entity, components::Layer& layer)
    {
      assert (m_ordered_layers.find (layer.Order) == m_ordered_layers.end() &&
              "You should assign different orders for layers.");
      assert (m_layers.find (layer.Id) == m_layers.end() && "You should assign different IDs for layers.");

      if (layer.ShaderId == "bloom")
      {
        m_layers[layer.Id] = std::shared_ptr<graphics::FireLayer> (new graphics::FireLayer (this->m_display));
      }
      else if (!layer.frag_shader_source.empty() && !layer.vert_shader_source.empty())
      {
        m_layers[layer.Id] =
            std::shared_ptr<graphics::BasicLayer> (new graphics::BasicLayer (this->m_display.GetWidth(),
                                                                             this->m_display.GetHeight(),
                                                                             layer.vert_shader_source.c_str(),
                                                                             layer.frag_shader_source.c_str(),
                                                                             layer.Fixed));
      }
      else
      {
        m_layers[layer.Id] =
            std::shared_ptr<graphics::BasicLayer> (new graphics::BasicLayer (this->m_display.GetWidth(),
                                                                             this->m_display.GetHeight(),
                                                                             "assets/shaders/sprite_batch.vert",
                                                                             "assets/shaders/sprite_batch.frag",
                                                                             layer.Fixed));
      }
      m_ordered_layers[layer.Order] = layer.Id;
    }
  };
} // namespace systems
} // namespace stella
