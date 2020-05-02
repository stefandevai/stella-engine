#pragma once

#include "./system.hpp"
#include "stella/graphics/layers/sprite_layer2.hpp"
#include "stella/core/resource.hpp"
namespace stella{ namespace graphics{ class Texture; } }

namespace stella
{
namespace system
{
  using TexRes = stella::core::ResourceManager<stella::graphics::Texture, const std::string>;

  class RenderT : public System
  {
    public:
      RenderT(entt::registry& registry, TexRes& textures);
      void update (entt::registry& registry, const double dt) override;
      // TEMP
      graphics::SpriteLayerT test_layer{"assets/shaders/sprite_batch.vert", "assets/shaders/sprite_batch.frag"};
      // TEMP

    private:
      TexRes& m_textures;
      void m_add_sprite_to_layer (entt::registry& registry, entt::entity entity);
      void m_init_sprite (entt::registry& registry, entt::entity entity);
      void m_update_sprite (entt::registry& registry, entt::entity entity);
      void m_destroy_sprite (entt::registry& registry, entt::entity entity);
  };
} // namespace system
} // namespace stella
