#include "stella/physics2d/grid_world.h"
#include "stella/physics2d/body.h"
#include <algorithm>
#include <cmath>
#include <iostream>

namespace stella {
namespace physics2d {

  GridWorld::GridWorld(const core::TileMap &tile_map)
    : m_tile_map(tile_map)
  {
  }

  GridWorld::~GridWorld() {
    this->Bodies.clear();
  }

  void GridWorld::Update(float dt) {
    this->UpdateMovement(dt);
    this->UpdateCollisions();
  }

  void GridWorld::AddBody(std::shared_ptr<stella::physics2d::Body> body) {
    this->Bodies.emplace_back(body);
  }

  void GridWorld::RemoveBody(std::shared_ptr<stella::physics2d::Body> body) {
    auto it = std::find(this->Bodies.begin(), this->Bodies.end(), body);
    if (it != this->Bodies.end())
      this->Bodies.erase(it);
  }

  void GridWorld::UpdateCollisions()
  {
    for (const auto &body: this->Bodies)
    {
      body->Collisions.reset();
      if (!body->IsStatic)
      {
        resolve_collisions(body);
      }
    }
  }

  void GridWorld::resolve_collisions(const std::shared_ptr<Body> &body)
  {
    const int bx = body->Position.x / TILE_DIMENSIONS;
    const int by = body->Position.y / TILE_DIMENSIONS;
    const int bw = (body->Position.x + body->Dimension.x) / TILE_DIMENSIONS;
    const int bh = (body->Position.y + body->Dimension.y) / TILE_DIMENSIONS;

    if (body->LastPosition.x >= body->Position.x)
    {
      check_collisions(body, bw, bx-1, by-1, bh);
    }
    else
    {
      check_collisions(body, bx-1, bw+1, by-1, bh);
    }
  }

  void GridWorld::check_collisions(const std::shared_ptr<Body> &body, int beginx, int endx, int beginy, int endy)
  {
    for (const auto &layer : m_tile_map.collision_layers)
    {
      for (auto i = beginx;;)
      {
        for (auto j = beginy;;)
        {
          const int tile_value = layer->get_value(i, j);
          if (tile_value != 0 && tile_value != -1)
          {
            Collision collision{body, Tile{tile_value,i,j}};
            const auto intersection = get_tile_intersection(collision);
            if (intersection.x*intersection.y > 0)
            {
              collision.intersection = intersection;
              resolve_collision(collision);
            }
          }

          if (beginy <= endy)
          {
            ++j;
            if (j > endy) break;
          }
          else
          {
            --j;
            if (j < endy) break;
          }
        }

        if (beginx <= endx)
        {
          ++i;
          if (i > endx) break;
        }
        else
        {
          --i;
          if (i < endx) break;
        }
      }
    }
  }

  glm::vec2 GridWorld::get_tile_intersection(const GridWorld::Collision &collision)
  {
    const int bx = collision.body->Position.x;
    const int by = collision.body->Position.y;
    const int bw = collision.body->Position.x + collision.body->Dimension.x;
    const int bh = collision.body->Position.y + collision.body->Dimension.y;

    const int tx = collision.tile.x * TILE_DIMENSIONS;
    const int ty = collision.tile.y * TILE_DIMENSIONS;
    const int tw = collision.tile.x * TILE_DIMENSIONS + TILE_DIMENSIONS;
    const int th = collision.tile.y * TILE_DIMENSIONS + TILE_DIMENSIONS;

    int intersection_x = 0;
    int intersection_y = 0;

    // Right intersection
    if (bx <= tx && bw >= tx)
    {
      //intersection_x = bw - tx;
      intersection_x = std::min<int>(abs(bw - tx), TILE_DIMENSIONS);
    }
    // Left intersection
    else if (bw > tw && bx < tw)
    {
      intersection_x = tw - bx;
      //if (intersection_x < 0 || intersection_x > 32)
        //std::cout << "2 " << intersection_x << '\n';
    }
    // Bottom intersection
    if (by <= ty && bh >= ty)
    {
      intersection_y = bh - ty + 1;
    }
    // Top intersection
    else if (bh >= th && by <= th)
    {
      intersection_y = th - by;
    }

    return glm::vec2(intersection_x, intersection_y);
  }

  void GridWorld::resolve_collision(const GridWorld::Collision &collision)
  {
    switch(collision.tile.type)
    {
      case 1:
        resolve_aabb(collision);
        break;
      case 2:
        resolve_tb_slope45(collision);
        break;
      case 3:
        resolve_bt_slope45(collision);
        break;
    }
  }

  void GridWorld::resolve_aabb(const GridWorld::Collision &collision)
  {
    const int lx = collision.body->LastPosition.x;
    const int ly = collision.body->LastPosition.y;
    const int lw = collision.body->LastPosition.x + collision.body->Dimension.x;
    const int lh = collision.body->LastPosition.y + collision.body->Dimension.y;

    const int bx = collision.body->Position.x;
    const int by = collision.body->Position.y;
    const int bw = collision.body->Position.x + collision.body->Dimension.x;
    const int bh = collision.body->Position.y + collision.body->Dimension.y;

    const int tx = collision.tile.x * TILE_DIMENSIONS;
    const int ty = collision.tile.y * TILE_DIMENSIONS;
    const int tw = collision.tile.x * TILE_DIMENSIONS + TILE_DIMENSIONS;
    const int th = collision.tile.y * TILE_DIMENSIONS + TILE_DIMENSIONS;

    const int ix = collision.intersection.x;
    const int iy = collision.intersection.y;

    // Right collision
    if (lw <= tw && lw <= bw && iy > ix)
    {
      collision.body->Collisions.set(1);
      collision.body->Position.x -= collision.intersection.x;
      //collision.body->Velocity.x = 0;
      //collision.body->Acceleration.x = 0;
    }
    // Left collision
    else if (lx >= tx && lx > bx && iy > ix)
    {
      collision.body->Collisions.set(3);
      collision.body->Position.x += collision.intersection.x;
      //collision.body->Velocity.x = 0;
      //collision.body->Acceleration.x = 0;
    }
    // Bottom collision
    else if (lh <= th && lh <= bh && ix > iy)
    {
      collision.body->Collisions.set(2);
      collision.body->Position.y -= collision.intersection.y - 1;

      //if (m_grid.get_value(collision.tile.x-1, collision.tile.y) != 1 && lx > bx)
      //{
        //collision.body->Velocity.y = collision.body->TargetVelocity.y;
        //collision.body->Acceleration.y = this->Gravity;
      //}
      //else if (m_grid.get_value(collision.tile.x+1, collision.tile.y) != 1 && lx < bx)
      //{
        //collision.body->Velocity.y = collision.body->TargetVelocity.y;
        //collision.body->Acceleration.y = this->Gravity;
      //}
      //else
      //{
        //collision.body->Velocity.y = 0;
        //collision.body->Acceleration.y = 0;
      //}
    }
    // Top collision
    else if (ly >= ty && ly > by && ix > iy)
    {
      collision.body->Collisions.set(0);
      collision.body->Position.y += collision.intersection.y;
      collision.body->Velocity.y = 0;
      collision.body->Acceleration.y = 0;
    }
    else
    {
      // Particular cases
      // I still don't understand why they occur
      if (ix == iy)
      {
        // Do nothing
      }
      else if (ly < by)
      {
        collision.body->Position.y -= iy-1;
        collision.body->Collisions.set(2);
        //collision.body->Velocity.y = 0;
        //collision.body->Acceleration.y = 0;
        //std::cout << "goes wronggggg\n";
      }
      else if (ly > by)
      {
        collision.body->Position.y += iy;
        collision.body->Collisions.set(0);
        //collision.body->Velocity.y = 0;
        //collision.body->Acceleration.y = 0;
        //std::cout << "wronggggg\n";
      }
      else
      {
        std::cout << "Uncaught collision: Should not be here\n";
        std::cout << "intersections: " << ix << ' ' << iy << '\n';
        std::cout << "right: " << (lw <= tw) << '\n';
        std::cout << "left: " << (lx >= tx) << '\n';
        std::cout << "bottom: " << (lh <= th) << '\n';
        std::cout << "top: " << (ly >= ty) << '\n';
      }
    }
  }

  void GridWorld::resolve_tb_slope45(const GridWorld::Collision &collision)
  {
    const int bx = collision.body->Position.x;
    const int tx = collision.tile.x * TILE_DIMENSIONS;
    //const int bw = collision.body->Position.x + collision.body->Dimension.x;
    //const int tw = collision.tile.x * TILE_DIMENSIONS + TILE_DIMENSIONS;

    if (bx >= tx)
    {
      const int bh = collision.body->Position.y + collision.body->Dimension.y;
      const int th = collision.tile.y * TILE_DIMENSIONS + TILE_DIMENSIONS;
      const int ix = collision.intersection.x;

      if (bh >= th - ix)
      {
        const int delta = bh - th + ix;
        collision.body->Collisions.set(2);
        collision.body->Position.y -= delta;

        // Checks if there isn't a same slope tile to the left, otherwise keep max velocity and acceleration
        // This is needed to allow a smooth walk through the tile
        //std::cout << delta << '\n';
        //std::cout << ix << '\n';
        //std::cout << '\n';
        //std::cout << '\n';
        //if (ix == TILE_DIMENSIONS && m_grid.get_value(collision.tile.x - 1, collision.tile.y - 1) != 2)
        //{
          //std::cout << delta << '\n';
          //collision.body->Velocity.y = 0;
          //collision.body->Acceleration.y = 0;
        //}
        //else
        //{
          //collision.body->Acceleration.y = this->Gravity;
          //collision.body->Velocity.y = collision.body->TargetVelocity.y;
        //}
      }
    }
    else
    {
      const int iy = collision.intersection.y;
      collision.body->Position.y -= iy-1;
      //collision.body->Acceleration.y = this->Gravity;
      //collision.body->Velocity.y = collision.body->TargetVelocity.y;
    }
  }

  void GridWorld::resolve_bt_slope45(const GridWorld::Collision &collision)
  {
    const int bw = collision.body->Position.x + collision.body->Dimension.x;
    const int tw = collision.tile.x * TILE_DIMENSIONS + TILE_DIMENSIONS;

    if (bw <= tw)
    {
      const int bh = collision.body->Position.y + collision.body->Dimension.y;
      const int th = collision.tile.y * TILE_DIMENSIONS + TILE_DIMENSIONS;
      const int ix = collision.intersection.x;

      if (bh >= th - ix)
      {
        const int delta = bh - th + ix;
        collision.body->Collisions.set(2);
        collision.body->Position.y -= delta;

        // Checks if there isn't a same slope tile to the right, otherwise keep max velocity and acceleration
        // This is needed to allow a smooth walk through the tile
        //if (ix == TILE_DIMENSIONS && m_grid.get_value(collision.tile.x + 1, collision.tile.y + 1) != 2)
        //{
          //collision.body->Velocity.y = 0;
          //collision.body->Acceleration.y = 0;
        //}
        //else
        //{
          //collision.body->Acceleration.y = this->Gravity;
          //collision.body->Velocity.y = collision.body->TargetVelocity.y;
        //}
      }
    }
    else
    {
      const int iy = collision.intersection.y;
      collision.body->Position.y -= iy-1;
      //collision.body->Acceleration.y = this->Gravity;
      //collision.body->Velocity.y = collision.body->TargetVelocity.y;
    }
  }

  void GridWorld::UpdateMovement(float dt) const {
    for (auto& body: this->Bodies) {
      if (!body->IsStatic) {
        body->LastPosition = body->Position;
        // X movement
        if (fabs(body->Acceleration.x) > 0.f) {
          if (fabs(body->Velocity.x + body->Acceleration.x*dt) <= body->TargetVelocity.x) {
            body->Velocity.x += body->Acceleration.x*dt;
          }
          else {
            body->Velocity.x = body->TargetVelocity.x*fabs(body->Acceleration.x)/body->Acceleration.x;
          }
        }
        else {
          if (fabs(body->Velocity.x) - body->Drag.x*dt > 0.f) {
            body->Velocity.x -= body->Drag.x*dt*body->Velocity.x/fabs(body->Velocity.x);
          }
          else body->Velocity.x = 0.f;
        }
        body->Position.x += body->Velocity.x*dt;

        // Y movement
        if (body->Gravity) body->Acceleration.y += this->Gravity*dt;
        if (body->Gravity && body->Acceleration.y > this->Gravity) body->Acceleration.y = this->Gravity;

        if (fabs(body->Acceleration.y) > 0.f) {
          if (fabs(body->Velocity.y) < body->TargetVelocity.y) {
            body->Velocity.y += body->Acceleration.y*dt;
          }
          else {
            body->Velocity.y = body->TargetVelocity.y*(body->Acceleration.y/fabs(body->Acceleration.y));
          }
        }
        else {
          if (fabs(body->Velocity.y) - body->Drag.y*dt > 0.f) {
            body->Velocity.y -= body->Drag.y*dt*body->Velocity.y/fabs(body->Velocity.y);
          }
          else body->Velocity.y = 0.f;
        }
        body->Position.y += body->Velocity.y*dt;
      }

      if (body->CollideWithBorders)
      {
        if (body->Position.x < 0.f)
        {
          body->Position.x = 0.f;
        }
        else if (body->Position.y < 0.f)
        {
          body->Position.y = 0.f;
        }

        //if (body->Position.x < 0.f + this->CameraOffset.x)
        //{
          //body->Position.x = 0.f + this->CameraOffset.x;
        //}
        //else if (body->Position.x + body->Dimension.x > this->CameraOffset.x + this->ScreenWidth)
        //{
          //body->Position.x = this->CameraOffset.x + this->ScreenWidth - body->Dimension.x;
        //}

        //if (body->Position.y < 0.f + this->CameraOffset.y)
        //{
          //body->Position.y = 0.f + this->CameraOffset.y;
        //}
        //else if (body->Position.y + body->Dimension.y > this->CameraOffset.y + this->ScreenHeight)
        //{
          //body->Position.y = this->CameraOffset.y + this->ScreenHeight - body->Dimension.y;
          ////body->Collisions.set(2);
        //}
      }
    }
  }

}
}
