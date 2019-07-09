#include "stella/physics2d/grid_world.h"
#include "stella/physics2d/body.h"
#include <algorithm>
#include <cmath>
#include <iostream>

namespace stella {
namespace physics2d {

  GridWorld::GridWorld()
  {
    for (auto i = 0; i < 28; ++ i)
      m_grid.set_value(i, 14, 1);
    m_grid.set_value(2, 13, 1);
    m_grid.set_value(4, 13, 1);
    m_grid.set_value(7, 13, 1);
    m_grid.set_value(8, 13, 1);
    m_grid.set_value(10, 12, 1);
    m_grid.set_value(11, 11, 1);
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

  void GridWorld::resolve_collisions(const std::shared_ptr<Body> body)
  {
    Collision collision{body, Tile{0,0,0}};

    const int bx = body->Position.x / TILE_DIMENSIONS;
    const int by = body->Position.y / TILE_DIMENSIONS;
    const int bw = (body->Position.x + body->Dimension.x) / TILE_DIMENSIONS;
    const int bh = (body->Position.y + body->Dimension.y) / TILE_DIMENSIONS;

    for (auto i = bx-1; i <= bw; ++i)
    {
      for (auto j = by-1; j <= bh; ++j)
      {
        const int tile_value = m_grid.get_value(i, j);
        if (tile_value != 0 && tile_value != -1)
        {
          collision.tile.x = i;
          collision.tile.y = j;
          const auto intersection = get_tile_intersection(collision);
          if (intersection.x*intersection.y > 0)
          {
            collision.tile.type = tile_value;
            collision.intersection = intersection;
            resolve_collision(collision);
          }
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
      intersection_x = bw - tx;
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
    switch(collision.tile.type)
    {
      case 1:
        resolve_aabb(collision);
        break;
      //case 2:
        //resolve_tb_slope45(collision);
        //break;
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
    // Right intersection
    if (lw <= tw && lw <= bw && iy > ix)
    {
      collision.body->Collisions.set(1);

      collision.body->Position.x -= collision.intersection.x;
      collision.body->Velocity.x = 0;
      collision.body->Acceleration.x = 0;
    }
    // Left intersection
    else if (lx >= tx && lx > bx && iy > ix)
    {
      collision.body->Collisions.set(3);

      collision.body->Position.x += collision.intersection.x;
      collision.body->Velocity.x = 0;
      collision.body->Acceleration.x = 0;
    }
    // Bottom intersection
    else if (lh <= th && lh <= bh && ix > iy)
    {
      collision.body->Collisions.set(2);

      collision.body->Position.y -= collision.intersection.y - 1;
      collision.body->Velocity.y = 0;
      collision.body->Acceleration.y = 0;
    }
    // Top intersection
    else if (ly >= ty && ly > by && ix > iy)
    {
      collision.body->Collisions.set(0);

      collision.body->Position.y += collision.intersection.y;
      collision.body->Velocity.y = 0;
      collision.body->Acceleration.y = 0;
    }
    else
    {
      std::cout << "here\n";
    }

    // Bottom collision
    //if (collision.body->Collisions.test(2))
    //{
      //if (ix > iy)
      //{
        //collision.body->Position.y -= collision.intersection.y - 1;
        //collision.body->Velocity.y = 0;
        //collision.body->Acceleration.y = 0;
      //}
      //else
      //{
        //std::cout << 2 << '\n';
      //}
    //}
    //// Top collision
    //else if (collision.body->Collisions.test(0))
    //{
      //if (ix > iy)
      //{
        //collision.body->Position.y += collision.intersection.y;
        //collision.body->Velocity.y = 0;
        //collision.body->Acceleration.y = 0;
      //}
      //else
      //{
        //std::cout << 0 << '\n';
      //}
    //}
    //// Right collision
    //if (collision.body->Collisions.test(1))
    //{
      //if (iy > ix)
      //{
        //collision.body->Position.x -= collision.intersection.x;
        //collision.body->Velocity.x = 0;
        //collision.body->Acceleration.x = 0;
      //}
      //else
      //{
        //std::cout << 1 << '\n';
      //}
    //}
    //// Left collision
    //else if (collision.body->Collisions.test(3))
    //{
      //if (iy > ix)
      //{
        //collision.body->Position.x += collision.intersection.x;
        //collision.body->Velocity.x = 0;
        //collision.body->Acceleration.x = 0;
      //}
      //else
      //{
        //std::cout << 3 << '\n';
      //}
    //}
  }

  //void GridWorld::resolve_tb_slope45(const GridWorld::Collision &collision)
  //{

  //}

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
            //std::cout << "here\n";
            body->Velocity.x -= body->Drag.x*dt*body->Velocity.x/fabs(body->Velocity.x);
            //std::cout << body->Drag.x << std::endl;
            //std::cout << body->Velocity.x << " " << body->Drag.x*body->Velocity.x/fabs(body->Velocity.x) << std::endl;
          }
          else body->Velocity.x = 0.f;
        }
        body->Position.x += body->Velocity.x*dt;

        // Y movement
        if (body->Gravity) body->Acceleration.y += this->Gravity*dt;

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
        if (body->Position.x < 0.f + this->CameraOffset.x)
        {
          body->Position.x = 0.f + this->CameraOffset.x;
        }
        else if (body->Position.x + body->Dimension.x > this->CameraOffset.x + this->ScreenWidth)
        {
          body->Position.x = this->CameraOffset.x + this->ScreenWidth - body->Dimension.x;
        }

        if (body->Position.y < 0.f + this->CameraOffset.y)
        {
          body->Position.y = 0.f + this->CameraOffset.y;
        }
        else if (body->Position.y + body->Dimension.y > this->CameraOffset.y + this->ScreenHeight)
        {
          body->Position.y = this->CameraOffset.y + this->ScreenHeight - body->Dimension.y;
          //body->Collisions.set(2);
        }
      }
    }
  }

}
}
