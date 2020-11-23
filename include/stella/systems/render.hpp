#pragma once

#include "./system.hpp"
#include "stella/graphics/layers/sprite_layer.hpp"
#include "stella/core/resource.hpp"
#include <map>

namespace stella
{
namespace graphics
{
  class Texture;
}
} // namespace stella
namespace stella
{
namespace graphics
{
  class Font;
}
} // namespace stella

#include "stella/types.hpp"

namespace stella
{
namespace system
{
  // using TextureManager = stella::core::ResourceManager<stella::graphics::Texture, const std::string>;
  using LayerMap = std::unordered_map<std::string, std::shared_ptr<graphics::LayerT>>;

  class RenderT : public System
  {
  public:
    RenderT (entt::registry& registry, TextureManager& textures);
    void update (entt::registry& registry, const double dt);

  private:
    const unsigned int DEFAULT_LAYER_ORDER = 256;
    const std::string DEFAULT_LAYER_NAME   = "__DEFAULT_LAYER";

    // LayerMap m_layers{{DEFAULT_LAYER_NAME,
    //    std::make_shared<graphics::SpriteLayerT> ("assets/shaders/sprite_batch.vert",
    //                                              "assets/shaders/sprite_batch.frag")}};
    LayerMap m_layers{};
    std::map<int, std::string> m_ordered_layers{{DEFAULT_LAYER_ORDER, DEFAULT_LAYER_NAME}};
    TextureManager& m_textures;

    void m_add_renderable_to_layer (const std::string& layer_name, entt::entity entity);
    void m_add_sprite_to_layer (entt::registry& registry, entt::entity entity);
    void m_remove_renderable_from_layer (const std::string& layer_name, entt::entity entity);
    void m_init_layer (entt::registry& registry, entt::entity entity);
    void m_init_sprite (entt::registry& registry, entt::entity entity);
    void m_destroy_sprite (entt::registry& registry, entt::entity entity);
    void m_init_shape (entt::registry& registry, entt::entity entity);
    void m_destroy_shape (entt::registry& registry, entt::entity entity);
  };
} // namespace system
} // namespace stella
