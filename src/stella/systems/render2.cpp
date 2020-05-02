#include "stella/systems/render2.hpp"
#include "stella/components/sprite2.hpp"
#include "stella/components/position.hpp"
#include "stella/graphics/texture.hpp"

#include <iostream>

namespace stella
{
namespace system
{
    RenderT::RenderT(entt::registry& registry, TexRes& textures)
      : m_textures(textures)
    {
        registry.on_construct<component::SpriteT>().connect<&RenderT::m_init_sprite> (this);
        registry.on_update<component::SpriteT>().connect<&RenderT::m_update_sprite> (this);
        registry.on_destroy<component::SpriteT>().connect<&RenderT::m_destroy_sprite> (this);
    }

    void RenderT::update (entt::registry& registry, const double dt)
    {
        // TEMP
        // registry.group<component::SpriteT> (entt::get<component::Position>)
        // .each ([this, &registry] (auto entity, auto& sprite, auto& pos) {
        //     std::cout << sprite.texture << '\n';
        // });

        test_layer.render(registry);
        // TEMP
    }

    void RenderT::m_add_sprite_to_layer (entt::registry& registry, entt::entity entity)
    {
        auto& sprite = registry.get<component::SpriteT>(entity);
        if (!sprite.loaded && !sprite.texture.empty())
        {
            std::cout << sprite.texture << '\n';
            sprite.texture_ptr = m_textures.load(sprite.texture);
            test_layer.add(entity);
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
        test_layer.remove(entity);
    }

} // namespace system
} // namespace stella
