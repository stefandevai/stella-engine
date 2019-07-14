#include "stella/physics2d/grid_world.h"
#include "stella/physics2d/body.h"
#include "stella/physics2d/water_surface.h"
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

  //bool GridWorld::intersects(const std::shared_ptr<Body> &body, const core::Tile &tile)
  //{
    //const auto ax = body->Position.x;
    //const auto ay = body->Position.y;
    //const auto aw = body->Dimension.x;
    //const auto ah = body->Dimension.y;
    //const auto bx = tile.x*TILE_DIMENSIONS;
    //const auto by = tile.y*TILE_DIMENSIONS;
    //const auto bw = TILE_DIMENSIONS;
    //const auto bh = TILE_DIMENSIONS;

    //return (abs(ax - bx) < (aw + bw)) && (abs(ay - by) < (ah + bh - TILE_DIMENSIONS));
  //}

  void GridWorld::check_collisions(const std::shared_ptr<Body> &body, int beginx, int endx, int beginy, int endy)
  {
    for (const auto &layer : m_tile_map.collision_layers)
    {
      for (auto i = beginx;;)
      {
        for (auto j = beginy;;)
        {
          const auto &layer_tile= layer->get_value(i, j);
          const int tile_value = layer_tile.value;
          if (tile_value != 0 && tile_value != -1)
          {
            Collision collision{body, layer_tile};
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
    switch(collision.tile.value)
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
      // If tile has a active edge in the opposite direction of the body (left)
      if (collision.tile.active_edges.test(3))
      {
        collision.body->Collisions.set(1);
        collision.body->Position.x -= collision.intersection.x;
        //std::cout << "colliding right\n";
      }
    }
    // Left collision
    else if (lx >= tx && lx > bx && iy > ix)
    {
      // If tile has a active edge in the opposite direction of the body (right)
      if (collision.tile.active_edges.test(1))
      {
        collision.body->Collisions.set(3);
        collision.body->Position.x += collision.intersection.x;
        //std::cout << "colliding left\n";
      }
    }
    // Bottom collision
    else if (lh <= th && lh <= bh && ix > iy)
    {
      // If tile has a active edge in the opposite direction of the body (top)
      if (collision.tile.active_edges.test(0))
      {
        collision.body->Collisions.set(2);
        collision.body->Position.y -= collision.intersection.y - 1;
        //std::cout << "colliding bottom\n";
      }

      // If the next or previous tile is the air, set y velocity to zero
      if (collision.tile.active_edges.test(1) || collision.tile.active_edges.test(3))
      {
        collision.body->Velocity.y = 0.0f;
      }
    }
    // Top collision
    else if (ly >= ty && ly > by && ix > iy)
    {
      // If tile has a active edge in the opposite direction of the body (bottom)
      if (collision.tile.active_edges.test(2))
      {
        collision.body->Collisions.set(0);
        collision.body->Position.y += collision.intersection.y;
        collision.body->Velocity.y = 0;
        collision.body->Acceleration.y = 0;
        //std::cout << "colliding top\n";
      }
    }
    else
    {
    }
  }

  void GridWorld::resolve_tb_slope45(const GridWorld::Collision &collision)
  {
    const int bx = collision.body->Position.x;
    const int tx = collision.tile.x * TILE_DIMENSIONS;

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
      }
    }
    else
    {
      const int iy = collision.intersection.y;
      collision.body->Position.y -= iy-1;
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
      }
    }
    else
    {
      const int iy = collision.intersection.y;
      collision.body->Position.y -= iy-1;
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
        if (body->Gravity && !body->Collisions.test(2)) body->Acceleration.y += this->Gravity*dt;
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

      // Collide body with world borders if necessary
      if (body->CollideWithBorders)
      {
        if (body->Position.x < 0.f)
        {
          body->Position.x = 0.f;
        }
        else if (body->Position.x + body->Dimension.x > (m_tile_map.width()-1)*TILE_DIMENSIONS)
        {
          body->Position.x = (m_tile_map.width()-1) * TILE_DIMENSIONS - body->Dimension.x;
        }
        if (body->Position.y < 0.f)
        {
          body->Position.y = 0.f;
        }
        else if (body->Position.y + body->Dimension.y > (m_tile_map.height()-1)*TILE_DIMENSIONS)
        {
          body->Position.y = (m_tile_map.height()-1) * TILE_DIMENSIONS - body->Dimension.y;
        }
      }
    }
  }

}
}
