#include "stella/systems/render.hpp"
#include "stella/components/layer.hpp"
#include "stella/components/sprite.hpp"
#include "stella/components/shape.hpp"
#include "stella/components/position.hpp"
#include "stella/components/camera.hpp"
#include "stella/graphics/texture.hpp"
#include "stella/graphics/layers/shape_layer.hpp"

#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

namespace stella
{
namespace system
{
  RenderT::RenderT (entt::registry& registry, TextureManager& textures) : m_textures (textures)
  {
    // Create a default layer in case a sprite is added to an unknown layer
    m_layers[DEFAULT_LAYER_NAME] = std::make_shared<graphics::SpriteLayerT> (
        registry, "assets/shaders/sprite_batch.vert", "assets/shaders/sprite_batch.frag", false);
    registry.on_construct<component::LayerT>().connect<&RenderT::m_init_layer> (this);
    registry.on_construct<component::SpriteT>().connect<&RenderT::m_init_sprite> (this);
    registry.on_destroy<component::SpriteT>().connect<&RenderT::m_destroy_sprite> (this);
    registry.on_construct<component::Shape>().connect<&RenderT::m_init_shape> (this);
    registry.on_destroy<component::Shape>().connect<&RenderT::m_destroy_shape> (this);
  }

  void RenderT::update (entt::registry& registry, const double dt)
  {
    // TODO: Find another way to add to layer so we don't have to loop over all
    // sprites on every frame.
    registry.view<component::SpriteT>().each ([this, &registry] (auto entity, auto& sprite) {
      if (!sprite.loaded)
      {
        m_add_sprite_to_layer (registry, entity);
      }
    });
    registry.view<component::Shape>().each ([this, &registry] (auto entity, auto& shape) {
      if (!shape.in_layer)
      {
        m_add_renderable_to_layer (shape.layer_id, entity);
        shape.in_layer = true;
      }
    });

    const auto camera_entity                        = *registry.view<stella::component::Camera>().begin();
    std::unique_ptr<component::Position> camera_pos = nullptr;
    if (camera_entity != entt::null)
    {
      camera_pos = std::make_unique<component::Position> (registry.get<component::Position> (camera_entity));
    }

    for (auto const& order : m_ordered_layers)
    {
      if (!m_layers[order.second]->fixed && camera_pos)
      {
        m_layers[order.second]->set_view_matrix (glm::lookAt (glm::vec3 (camera_pos->x, camera_pos->y, 30.0f),
                                                         glm::vec3 (camera_pos->x, camera_pos->y, -1.f),
                                                         glm::vec3 (0.f, 1.f, 0.f)));
      }
      m_layers[order.second]->render (registry);
    }
  }
  void RenderT::m_add_renderable_to_layer (const std::string& layer_name, entt::entity entity)
  {
    auto layer = m_layers.find (layer_name);
    if (layer != m_layers.end())
    {
      layer->second->add (entity);
    }
    else
    {
      std::cout << "WARNING: Could not find layer \"" << layer_name << "\". Adding sprite to default layer.\n";
      m_layers[DEFAULT_LAYER_NAME]->add (entity);
    }
  }

  void RenderT::m_remove_renderable_from_layer (const std::string& layer_name, entt::entity entity)
  {
    auto layer = m_layers.find (layer_name);
    if (layer != m_layers.end())
    {
      m_layers[layer_name]->remove (entity);
    }
    else
    {
      std::cout << "WARNING: Could not remove sprite from unknown layer " << layer_name << '\n';
      m_layers[DEFAULT_LAYER_NAME]->remove (entity);
    }
  }

  void RenderT::m_add_sprite_to_layer (entt::registry& registry, entt::entity entity)
  {
    auto& sprite = registry.get<component::SpriteT> (entity);
    if (!sprite.loaded && !sprite.texture.empty() && !sprite.layer.empty() &&
        registry.has<component::Position> (entity) && registry.has<component::Dimension> (entity))
    {
      m_add_renderable_to_layer (sprite.layer, entity);

      // If texture_ptr is not null, we already initialized a texture,
      // most likely a font atlas texture.
      if (!sprite.texture_ptr)
      {
        sprite.texture_ptr = m_textures.load (sprite.texture);
      }
      sprite.loaded = true;
    }
  }

  void RenderT::m_init_sprite (entt::registry& registry, entt::entity entity)
  {
    m_add_sprite_to_layer (registry, entity);
  }
  void RenderT::m_destroy_sprite (entt::registry& registry, entt::entity entity)
  {
    auto& sprite = registry.get<component::SpriteT> (entity);
    m_remove_renderable_from_layer (sprite.layer, entity);
  }

  void RenderT::m_init_shape (entt::registry& registry, entt::entity entity)
  {
    auto& shape = registry.get<component::Shape> (entity);
    if (!shape.in_layer)
    {
      m_add_renderable_to_layer (shape.layer_id, entity);
      shape.in_layer = true;
    }
  }
  void RenderT::m_destroy_shape (entt::registry& registry, entt::entity entity)
  {
    auto& shape = registry.get<component::Shape> (entity);
    m_remove_renderable_from_layer (shape.layer_id, entity);
  }

  void RenderT::m_init_layer (entt::registry& registry, entt::entity entity)
  {
    auto& layer = registry.get<component::LayerT> (entity);
    assert (m_ordered_layers.find (layer.order) == m_ordered_layers.end() &&
            "You should assign different orders for layers.");
    assert (m_layers.find (layer.id) == m_layers.end() && "You should assign different IDs for layers.");

    switch (layer.layer_type)
    {
      case component::LayerType::SPRITE_LAYER:
        m_layers[layer.id] = std::make_shared<graphics::SpriteLayerT> (
            registry, layer.vert_shader_source, layer.frag_shader_source, layer.fixed);
        break;
      case component::LayerType::SHAPE_LAYER:
        m_layers[layer.id] = std::make_shared<graphics::ShapeLayerT> (
            registry, layer.vert_shader_source, layer.frag_shader_source, layer.fixed);
        break;
      default:
        std::cout << "ERROR: Could not create layer. Undefined layer type.\n";
        break;
    }
    m_ordered_layers[layer.order] = layer.id;
  }
} // namespace system
} // namespace stella
