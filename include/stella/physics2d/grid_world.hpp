#pragma once

#include "../core/grid.h"
#include "../core/tile.h"
#include "../core/tile_map.h"
#include <glm/glm.hpp>
#include <memory>
#include <vector>

namespace stella
{
namespace physics2d
{
  class Body;
  class WaterSurface;

  class GridWorld
  {
  private:
    float Gravity = 1200.f;
    std::vector<std::shared_ptr<stella::physics2d::Body>> Bodies;
    glm::vec2 CameraOffset         = glm::vec2 (0.f, 0.f);
    const unsigned TILE_DIMENSIONS = 32;
    const core::TileMap& m_tile_map;
    std::vector<std::shared_ptr<stella::physics2d::WaterSurface>> m_water_surfaces;

    struct Collision
    {
      Collision (std::shared_ptr<Body> body, core::Tile tile) : body (body), tile (tile) {}
      std::shared_ptr<Body> body;
      core::Tile tile;
      glm::vec2 intersection = glm::vec2 (0.f, 0.f);
    };

  public:
    explicit GridWorld (const core::TileMap& tile_map);
    ~GridWorld();
    void AddBody (std::shared_ptr<stella::physics2d::Body> body);
    void RemoveBody (std::shared_ptr<stella::physics2d::Body> body);
    void Update (float dt);
    void SetGravity (float new_gravity) { this->Gravity = new_gravity; }
    inline void SetCameraOffset (float x, float y) { this->CameraOffset = glm::vec2 (x, y); }
    void add_water_surface (const std::shared_ptr<stella::physics2d::WaterSurface>& water_surface);
    void remove_water_surface (const std::shared_ptr<stella::physics2d::WaterSurface>& water_surface);

  private:
    void UpdateMovement (float dt) const;
    void UpdateCollisions();
    void resolve_collisions (const std::shared_ptr<Body>& body);
    void check_collisions (const std::shared_ptr<Body>& body, int beginx, int endx, int beginy, int endy);
    glm::vec2 get_tile_intersection (const GridWorld::Collision& collision);
    void resolve_collision (const GridWorld::Collision& collision);
    void resolve_aabb (const GridWorld::Collision& collision);
    void resolve_tb_slope45 (const GridWorld::Collision& collision);
    void resolve_bt_slope45 (const GridWorld::Collision& collision);
    // bool intersects(const std::shared_ptr<Body> &body, const core::Tile
    // &tile);
  };

} // namespace physics2d
} // namespace stella
