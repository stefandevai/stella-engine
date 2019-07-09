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
        Collision collision = is_colliding_with(body);
        if (collision.tile.type != 0)
        {
          resolve_collision(collision);
        }
      }
    }
  }

  GridWorld::Collision GridWorld::is_colliding_with(const std::shared_ptr<Body> body)
  {
    Collision collision{body, Tile{0,0,0}};
    Collision final_collision{body, Tile{0,0,0}};
    double intersection_area = 0.0;

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
          //if (abs(intersection.x*intersection.y) > intersection_area)
          if (abs(intersection.x*intersection.y) > 0)
          {
            intersection_area = intersection.x*intersection.y;
            collision.tile.type = tile_value;
            collision.intersection = intersection;
            resolve_collision(collision);
            //final_collision = collision;
          }
        }
      }
    }
    return final_collision;
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
      //std::cout << intersection_x << '\n';
      //std::cout << collision.body->LastPosition.x << '\n';
      //std::cout << collision.body->LastPosition.y << '\n';
      //std::cout << '\n';
    }
    // Bottom intersection
    if (by <= ty && bh >= ty)
    {
      intersection_y = bh - ty;
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
    //if (bx <= tx && bw >= tx && lw <= tx)
    //if (bx <= tx && bw >= tx && lw <= tx && (lh >= ty && ly <= th))
    //if (lw <= tx && (lh >= ty && ly <= th))
    if (lw <= tw && lw <= bw && iy > ix)
    {
      collision.body->Collisions.set(1);
    }
    // Left intersection
    //else if (bw >= tw && bx <= tw && lx >= tw)
    //else if (bw >= tw && bx <= tw && lx >= tw && (lh >= ty && ly <= th))
    //else if (lx >= tw && (lh >= ty && ly <= th))
    else if (lx >= tx && lx > bx && iy > ix)
    {
      collision.body->Collisions.set(3);
    }
    // Bottom intersection
    //else if (by <= ty && bh >= ty && lh <= ty)
    //else if (by <= ty && bh >= ty && lh <= ty && (lw >= tx && lx <= tw))
    //else if (by <= ty && bh >= ty)
    //else if (lh <= ty && (lw >= tx && lx <= tw))
    else if (lh <= th && lh < bh && ix > iy)
    {
      //std::cout << ix << ' ' << iy << '\n';
      collision.body->Collisions.set(2);
    }
    // Top intersection
    //else if (bh >= th && by <= th && ly >= th)
    //else if (bh >= th && by <= th && ly >= th && (lw >= tx && lx <= tw))
    //else if (bh >= th && by <= th)
    //else if (ly >= th && (lw >= tx && lx <= tw))
    else if (ly >= ty && ly > by && ix > iy)
    {
      collision.body->Collisions.set(0);
    }
    else
    {
      std::cout << "here\n";
      //std::cout << collision.intersection.x << '\n';
      //std::cout << collision.intersection.y << '\n';
      //std::cout << "here\n";
    }
    //if (collision.intersection.x > 0.f && collision.intersection.y > 0.f)
    //{
      //if (abs(collision.intersection.x) > abs(collision.intersection.y))
      //{
        //collision.body->Collisions.reset(1);
        //collision.body->Collisions.reset(3);

        // Bottom collision
        if (collision.body->Collisions.test(2))
        {
          if (ix > iy)
          {
            collision.body->Position.y -= collision.intersection.y;
            collision.body->Velocity.y = 0;
            collision.body->Acceleration.y = 0;
          }
          else
          {
            std::cout << ix << ' ' << iy << '\n';
          }
          //std::cout << "bottom collision\n";
        }
        // Top collision
        else if (collision.body->Collisions.test(0))
        {
          if (ix > iy)
          {
            collision.body->Position.y += collision.intersection.y;
            collision.body->Velocity.y = 0;
            collision.body->Acceleration.y = 0;
          }
          else
          {
            std::cout << ix << ' ' << iy << '\n';
          }
          //std::cout << "top collision\n";
        }
      //}
      //else 
      //{
        //collision.body->Collisions.reset(0);
        //collision.body->Collisions.reset(2);

        // Right collision
        if (collision.body->Collisions.test(1))
        {
          if (iy > ix)
          {
            collision.body->Position.x -= collision.intersection.x;
            //std::cout << "right collision\n";
            collision.body->Velocity.x = 0;
            collision.body->Acceleration.x = 0;
          }
          else
          {
            std::cout << ix << ' ' << iy << '\n';
          }
        }
        // Left collision
        else if (collision.body->Collisions.test(3))
        {
          if (iy > ix)
          {
            collision.body->Position.x += collision.intersection.x;
            //std::cout << "left collision\n";
            collision.body->Velocity.x = 0;
            collision.body->Acceleration.x = 0;
          }
          else
          {
            std::cout << ix << ' ' << iy << '\n';
          }
        }
      //}
    //}
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
