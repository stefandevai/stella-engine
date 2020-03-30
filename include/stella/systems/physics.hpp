#pragma once

#include "system.h"
#include "stella/components/body2d.h"
#include "stella/topdown/world.h"

namespace stella
{
namespace system
{
  class Physics : public System
  {
  private:
    const core::TileMap& m_tile_map;
    stella::topdown::World m_world{m_tile_map};

  public:
    Physics (const core::TileMap& tile_map, entt::registry& registry);
    void update (entt::registry& registry, const double dt) override;

  private:
    Physics() = delete;
    void remove_body_from_world (entt::registry& registry, entt::entity entity);
    void initialize_body (entt::registry& registry, entt::entity entity);
  };
} // namespace system
} // namespace stella
