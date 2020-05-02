#include "stella/systems/render.hpp"

#include "stella/components/camera.hpp"
#include "stella/components/dimension.hpp"
#include "stella/components/position.hpp"
#include "stella/components/transform.hpp"
#include "stella/components/shape.hpp"
#include "stella/components/color.hpp"
#include "stella/components/fog.hpp"
#include "stella/graphics/layers/firelayer.hpp"
#include "stella/graphics/layers/shape_layer.hpp"
#include <ctime>

namespace stella
{
namespace system
{
  Render::Render (entt::registry& registry,
                  core::ResourceManager<graphics::Texture, const std::string>& textures,
                  graphics::Display& display)
    : m_textures (textures), m_display (display)
  {
    registry.on_construct<component::Layer>().connect<&Render::initialize_layer> (this);
    registry.on_construct<component::Sprite>().connect<&Render::initialize_sprite> (this);
    registry.on_construct<component::Shape>().connect<&Render::initialize_shape> (this);
    registry.on_destroy<component::Sprite>().connect<&Render::remove_sprite_from_layer> (this);
    std::srand (static_cast<unsigned> (std::time (nullptr)));
  }

  void Render::update (entt::registry& registry, const double dt)
  {
    registry.group<component::Sprite> (entt::get<component::Position, component::Dimension>)
        .each ([this, &registry] (auto entity, auto& sprite, auto& pos, auto& dim) {
          // Sprite inLayer and position changed
          if (pos.has_changed())
          {
            m_layers[sprite.LayerId]->Remove (sprite.sprite);
            sprite.InLayer = false;

            sprite.sprite->Pos.x = (int) pos.x;
            sprite.sprite->Pos.y = (int) pos.y;
            sprite.sprite->Pos.z = (int) pos.z;
          }

          if (dim.has_changed())
          {
            m_layers[sprite.LayerId]->Remove (sprite.sprite);
            sprite.InLayer = false;

            sprite.sprite->Dimensions.x = dim.w;
            sprite.sprite->Dimensions.y = dim.h;
          }

          pos.last_x = pos.x;
          pos.last_y = pos.y;
          pos.last_z = pos.z;
          dim.last_w = dim.w;
          dim.last_h = dim.h;
          if (!sprite.InLayer && sprite.sprite != nullptr)
          {
            m_layers[sprite.LayerId]->Add (sprite.sprite);
            sprite.InLayer = true;
          }
          if (sprite.sprite == nullptr)
          {
            // std::cout << pos.x << '\n';
            // std::cout << pos.y << '\n';
            // std::cout << sprite.LayerId << '\n';
            // std::cout << sprite.Frame << '\n';
            // std::cout << sprite.InLayer << '\n';
            // std::cout << sprite.Initialized << '\n';
            // std::cout << sprite.TexName << '\n';
            // std::cout << '\n';
          }
        });

    const auto camera_entity = *registry.view<stella::component::Camera>().begin();
    auto& camera_pos         = registry.get<component::Position> (camera_entity);

    for (auto const& order : m_ordered_layers)
    {
      if (!m_layers[order.second]->Fixed)
      {
        m_layers[order.second]->SetViewMatrix (glm::lookAt (glm::vec3 (camera_pos.x, camera_pos.y, camera_pos.z),
                                                            glm::vec3 (camera_pos.x, camera_pos.y, camera_pos.z - 1.f),
                                                            glm::vec3 (0.f, 1.f, 0.f)));
      }
      m_layers[order.second]->Render();
    }
  }

  void Render::remove_sprite_from_layer (entt::registry& registry, entt::entity entity)
  {
    if (registry.has<component::Fog>(entity))
    {
      registry.remove<component::Fog>(entity);
    }

    auto& sprite = registry.get<component::Sprite> (entity);
    if (sprite.InLayer)
    {
      m_layers[sprite.LayerId]->Remove (sprite.sprite);
      sprite.InLayer = false;
    }
  }

  void Render::initialize_layer (entt::registry& registry, entt::entity entity)
  {
    auto& layer = registry.get<component::Layer> (entity);
    assert (m_ordered_layers.find (layer.Order) == m_ordered_layers.end() &&
            "You should assign different orders for layers.");
    assert (m_layers.find (layer.Id) == m_layers.end() && "You should assign different IDs for layers.");

    if (layer.ShaderId == "bloom")
    {
      m_layers[layer.Id] = std::shared_ptr<stella::graphics::FireLayer> (new graphics::FireLayer (this->m_display));
    }
    else if (layer.ShaderId == "shape" && !layer.frag_shader_source.empty() && !layer.vert_shader_source.empty())
    {
      m_layers[layer.Id] = std::shared_ptr<stella::graphics::ShapeLayer> (new graphics::ShapeLayer(this->m_display.GetWidth(),
                                                                                                   this->m_display.GetHeight(),
                                                                                                   layer.vert_shader_source.c_str(),
                                                                                                   layer.frag_shader_source.c_str(),
                                                                                                   layer.Fixed));
    }
    else if (!layer.frag_shader_source.empty() && !layer.vert_shader_source.empty())
    {
      m_layers[layer.Id] =
          std::shared_ptr<stella::graphics::BasicLayer> (new graphics::BasicLayer (this->m_display.GetWidth(),
                                                                                   this->m_display.GetHeight(),
                                                                                   layer.vert_shader_source.c_str(),
                                                                                   layer.frag_shader_source.c_str(),
                                                                                   layer.Fixed));
    }
    else
    {
      m_layers[layer.Id] =
          std::shared_ptr<stella::graphics::BasicLayer> (new graphics::BasicLayer (this->m_display.GetWidth(),
                                                                                   this->m_display.GetHeight(),
                                                                                   "assets/shaders/sprite_batch.vert",
                                                                                   "assets/shaders/sprite_batch.frag",
                                                                                   layer.Fixed));
    }
    m_ordered_layers[layer.Order] = layer.Id;
  }

  void Render::initialize_sprite (entt::registry& registry, entt::entity entity)
  {
    auto& sprite = registry.get<component::Sprite> (entity);
    // Adds sprite to layer
    if (!sprite.InLayer && sprite.Initialized)
    {
      m_layers[sprite.LayerId]->Add (sprite.sprite);
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
          const auto& pos = registry.get<component::Position> (entity);
          const auto& dim = registry.get<component::Dimension> (entity);
          // If no frame dimensions were provided
          if (sprite.FrameDimensions.x == 0)
          {
            sprite.sprite = std::shared_ptr<graphics::Sprite> (
                new graphics::Sprite (glm::vec3 (pos.x, pos.y, pos.z), *tex, sprite.Frame));
          }
          else
          {
            sprite.sprite = std::shared_ptr<graphics::Sprite> (
                new graphics::Sprite (glm::vec3 (pos.x, pos.y, pos.z),
                                      glm::vec2 (sprite.FrameDimensions.x, sprite.FrameDimensions.y),
                                      *tex,
                                      sprite.Frame));
          }
          if (registry.has<component::Transform> (entity))
          {
            const auto& trans = registry.get<component::Transform> (entity);
            sprite.sprite->SetDirectScale (glm::vec2 ((float) dim.w * trans.Scale.x, (float) dim.h * trans.Scale.y));
            sprite.sprite->SetRotation (trans.Rotation);
          }
          else
          {
            sprite.sprite->SetDirectScale (glm::vec2 ((float) dim.w, (float) dim.h));
          }
          sprite.Initialized = true;
        }
      }
    }
  }

  void Render::initialize_shape (entt::registry& registry, entt::entity entity)
  {
    auto& shape = registry.get<component::Shape> (entity);
    if (shape.shape && !shape.in_layer)
    {
      if (registry.has<component::Position>(entity))
      {
        auto& pos = registry.get<component::Position>(entity);
        shape.shape->Pos.x = pos.x;
        shape.shape->Pos.y = pos.y;
        shape.shape->Pos.z = pos.z;
      }
      m_layers[shape.layer_id]->Add (shape.shape);
      shape.in_layer = true;
    }
  }
} // namespace system
} // namespace stella
