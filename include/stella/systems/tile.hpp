#pragma once

#include "./system.hpp"

namespace stella
{
namespace core
{
  class TileMap;
}
} // namespace stella

namespace stella
{
namespace system
{
  class Tile : public System
  {
  private:
    core::TileMap& m_tile_map;
    entt::registry::entity_type m_camera;
    unsigned m_tile_dimension           = 32;
    int m_last_camera_x                 = 0;
    int m_last_camera_y                 = 0;
    const int m_frustrum_culling_offset = 1;

  public:
    Tile (core::TileMap& tilemap, entt::registry::entity_type camera, entt::registry& registry);
    void update (entt::registry& registry, const double dt) override;
    void remove_tile_visibility (entt::registry& registry, entt::entity entity);
  };
} // namespace system
} // namespace stella
