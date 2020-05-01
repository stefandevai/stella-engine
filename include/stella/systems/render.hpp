#pragma once

#include "stella/components/layer.hpp"
#include "stella/components/sprite.hpp"
#include "stella/core/resource.hpp"
#include "stella/systems/system.hpp"
#include "stella/graphics/display.hpp"
#include "stella/graphics/texture.hpp"
#include "stella/graphics/layers/layer.hpp"
#include "stella/graphics/layers/basic_layer.hpp"

namespace stella
{
namespace system
{
  class Render : public System
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
    Render (entt::registry& registry,
            core::ResourceManager<graphics::Texture, const std::string>& textures,
            graphics::Display& display);
    ~Render() override {}
    void update (entt::registry& registry, const double dt) override;

  private:
    Render() = delete;
    void initialize_sprite (entt::registry& registry, entt::entity entity);
    void initialize_shape (entt::registry& registry, entt::entity entity);
    void remove_sprite_from_layer (entt::registry& registry, entt::entity entity);
    void initialize_layer (entt::registry& registry, entt::entity entity);
  };
} // namespace system
} // namespace stella
