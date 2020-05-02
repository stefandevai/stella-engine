#pragma once

#include "./system.hpp"
#include <entt/entity/registry.hpp>
#include "stella/graphics/layers/shape_layer.hpp"

namespace stella
{
namespace system
{
  class Fog : public System
  {
  public:
    Fog (entt::registry& registry, const unsigned width, const unsigned height);
    void update (entt::registry& registry, const double dt) override;

  private:
    const float m_width, m_height;
    graphics::ShapeLayer m_fog_layer{m_width, m_height, "assets/shaders/fog.vert", "assets/shaders/fog.frag", false};

    Fog() = delete;
    void initialize_fog (entt::registry& registry, entt::entity entity);
  };
} // namespace system
} // namespace stella
