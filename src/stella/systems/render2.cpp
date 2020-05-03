#include "stella/systems/render2.hpp"
#include "stella/components/layer2.hpp"
#include "stella/components/sprite2.hpp"
#include "stella/components/position.hpp"
#include "stella/components/camera.hpp"
#include "stella/graphics/texture.hpp"

#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

namespace stella
{
namespace system
{
    RenderT::RenderT(entt::registry& registry, TexRes& textures)
      : m_textures(textures)
    {
        registry.on_construct<component::LayerT>().connect<&RenderT::m_init_layer> (this);
        registry.on_construct<component::SpriteT>().connect<&RenderT::m_init_sprite> (this);
        registry.on_update<component::SpriteT>().connect<&RenderT::m_update_sprite> (this);
        registry.on_destroy<component::SpriteT>().connect<&RenderT::m_destroy_sprite> (this);
    }

    void RenderT::update (entt::registry& registry, const double dt)
    {
        registry.view<component::SpriteT> ()
        .each ([this, &registry] (auto entity, auto& sprite) {
            if (!sprite.loaded)
            {
                m_add_sprite_to_layer(registry, entity);
            }
        });
        const auto camera_entity = *registry.view<stella::component::Camera>().begin();
        std::unique_ptr<component::Position> camera_pos = nullptr;
        if (camera_entity != entt::null)
        {
            camera_pos = std::make_unique<component::Position>(registry.get<component::Position> (camera_entity));
        }

        for (auto const& order : m_ordered_layers)
        {
            if (!m_layers[order.second]->fixed && camera_pos)
            {
                m_layers[order.second]->set_view_matrix (glm::lookAt (glm::vec3 (camera_pos->x, camera_pos->y, camera_pos->z),
                                                         glm::vec3 (camera_pos->x, camera_pos->y, camera_pos->z - 1.f),
                                                         glm::vec3 (0.f, 1.f, 0.f)));
            }
            m_layers[order.second]->render(registry);
        }
    }

    void RenderT::m_add_sprite_to_layer (entt::registry& registry, entt::entity entity)
    {
        auto& sprite = registry.get<component::SpriteT>(entity);
        if (!sprite.loaded && !sprite.texture.empty() && !sprite.layer.empty())
        {
            auto layer = m_layers.find(sprite.layer);
            if (layer != m_layers.end())
            {
                layer->second->add(entity);
            }
            else
            {
                std::cout << "WARNING: Could not find layer \"" << sprite.layer << "\". Adding sprite to default layer.\n";
                m_layers[DEFAULT_LAYER_NAME]->add(entity);
            }
            
            sprite.texture_ptr = m_textures.load(sprite.texture);
            sprite.loaded = true;
        }
    }

    void RenderT::m_init_sprite (entt::registry& registry, entt::entity entity)
    {
        m_add_sprite_to_layer(registry, entity);
    }

    void RenderT::m_update_sprite(entt::registry& registry, entt::entity entity)
    {
        m_add_sprite_to_layer(registry, entity);
    }

    void RenderT::m_destroy_sprite(entt::registry& registry, entt::entity entity)
    {
        auto& sprite = registry.get<component::SpriteT> (entity);
        auto layer = m_layers.find(sprite.layer);
        if (layer != m_layers.end())
        {
            if (m_layers[sprite.layer]->has(entity))
            {
                m_layers[sprite.layer]->remove (entity);
            }
        }
        else if (m_layers[DEFAULT_LAYER_NAME]->has(entity))
        {
            m_layers[DEFAULT_LAYER_NAME]->remove (entity);
        }
        else
        {
            std::cout << "WARNING: Could not remove sprite from unknown layer " << sprite.layer << '\n'; 
        }
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
                m_layers[layer.id] = std::make_shared<graphics::SpriteLayerT>(layer.vert_shader_source,
                                                                              layer.frag_shader_source,
                                                                              layer.fixed);
                break;
            default:
                std::cout << "ERROR: Could not create layer. Undefined layer type.\n";
                break;
        }
        m_ordered_layers[layer.order] = layer.id;
    }

} // namespace system
} // namespace stella
