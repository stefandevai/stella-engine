#pragma once

#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include "../core/grid.h"

namespace stella {
namespace physics2d {

  class Body;

  class GridWorld {
    private:
      float Gravity = 1200.f;
      std::vector<std::shared_ptr<stella::physics2d::Body>> Bodies;
      glm::vec2 CameraOffset = glm::vec2(0.f, 0.f);
      float ScreenWidth = 896.f;
      float ScreenHeight = 504.f;
      const unsigned TILE_DIMENSIONS = 32;
      core::Grid m_grid{static_cast<unsigned>(ScreenWidth)/TILE_DIMENSIONS, static_cast<unsigned>(ScreenHeight)/TILE_DIMENSIONS};

      struct Tile
      {
        Tile(int type, int x, int y) : type(type), x(x), y(y) {}
        int type, x, y;
      };

      struct Collision
      {
        Collision(std::shared_ptr<Body> body, Tile tile) : body(body), tile(tile) {}
        std::shared_ptr<Body> body;
        Tile tile;
        glm::vec2 intersection = glm::vec2(0.f,0.f);
      };

    public:
      GridWorld();
      ~GridWorld();
      void AddBody(std::shared_ptr<stella::physics2d::Body> body);
      void RemoveBody(std::shared_ptr<stella::physics2d::Body> body);
      void Update(float dt);
      void SetGravity(float new_gravity) { this->Gravity = new_gravity; }
      inline void SetCameraOffset(float x, float y) {
        this->CameraOffset = glm::vec2(x,y);
      }

    private:
      void UpdateMovement(float dt) const;
      void UpdateCollisions();
      void resolve_collisions(const std::shared_ptr<Body> body);
      glm::vec2 get_tile_intersection(const GridWorld::Collision &collision);
      void resolve_collision(const GridWorld::Collision &collision);

      void resolve_aabb(const GridWorld::Collision &collision);
      void resolve_tb_slope45(const GridWorld::Collision &collision);
      void resolve_bt_slope45(const GridWorld::Collision &collision);

  };

}
}
