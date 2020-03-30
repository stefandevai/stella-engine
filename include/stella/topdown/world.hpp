#pragma once

#include "../core/grid.hpp"
#include "../core/tile.hpp"
#include "../core/tile_map.hpp"
#include <glm/glm.hpp>
#include <memory>
#include <vector>

namespace stella
{
namespace topdown
{
  class Body;

  class World
  {
  private:
    std::vector<std::shared_ptr<stella::topdown::Body>> Bodies;
    glm::vec2 CameraOffset         = glm::vec2 (0.f, 0.f);
    const unsigned TILE_DIMENSIONS = 32;
    const core::TileMap& m_tile_map;

    struct Collision
    {
      Collision (std::shared_ptr<Body> body, core::Tile tile) : body (body), tile (tile) {}
      std::shared_ptr<Body> body;
      core::Tile tile;
      glm::vec2 intersection = glm::vec2 (0.f, 0.f);
    };

  public:
    explicit World (const core::TileMap& tile_map);
    ~World();
    void AddBody (std::shared_ptr<stella::topdown::Body> body);
    void RemoveBody (std::shared_ptr<stella::topdown::Body> body);
    void Update (float dt);

  private:
    void UpdateMovement (float dt) const;
    void UpdateCollisions();
    bool check_tile_collision (int x, int y);
    void resolve_collisions (const std::shared_ptr<Body>& body);
  };

} // namespace topdown
} // namespace stella