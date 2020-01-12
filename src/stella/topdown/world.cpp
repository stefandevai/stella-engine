#include "stella/topdown/world.h"
#include "stella/topdown/body.h"
#include <algorithm>
#include <cmath>
#include <iostream>

namespace stella {
namespace topdown {

  World::World(const core::TileMap &tile_map)
    : m_tile_map(tile_map)
  {
  }

  World::~World() {
    this->Bodies.clear();
  }

  void World::Update(float dt) {
    this->UpdateCollisions();
    this->UpdateMovement(dt);
  }

  void World::AddBody(std::shared_ptr<stella::topdown::Body> body) {
    this->Bodies.emplace_back(body);
  }

  void World::RemoveBody(std::shared_ptr<stella::topdown::Body> body) {
    auto it = std::find(this->Bodies.begin(), this->Bodies.end(), body);
    if (it != this->Bodies.end())
      this->Bodies.erase(it);
  }

  void World::UpdateCollisions()
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

  void World::resolve_collisions(const std::shared_ptr<Body> &body)
  {
    const int bx = body->Position.x / TILE_DIMENSIONS;
    const int by = body->Position.y / TILE_DIMENSIONS;
    const int bw = (body->Position.x + body->Dimension.x) / TILE_DIMENSIONS;
    const int bh = (body->Position.y + body->Dimension.y) / TILE_DIMENSIONS;

    if ((body->MovingTop() && check_tile_collision(bx, by-1))
    ||  (body->MovingRight() && check_tile_collision(bw+1, by))
    ||  (body->MovingBottom() && check_tile_collision(bx, bh+1))
    ||  (body->MovingLeft() && check_tile_collision(bx-1, by)))
    {
      body->ResetMovement();
    }

    /*if (body->LastPosition.x >= body->Position.x)
    {
      check_collisions(body, bw, bx-1, by-1, bh);
    }
    else
    {
      check_collisions(body, bx-1, bw+1, by-1, bh);
    }*/
  }

  bool World::check_tile_collision(int x, int y)
  {
    for (const auto &layer : m_tile_map.collision_layers)
    {
      if (layer->get_value(x, y).value > 0) return true;
    }
    return false;
  }

  void World::check_collisions(const std::shared_ptr<Body> &body, int beginx, int endx, int beginy, int endy)
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

  glm::vec2 World::get_tile_intersection(const World::Collision &collision)
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

  void World::resolve_collision(const World::Collision &collision)
  {
    switch(collision.tile.value)
    {
      case 0:
        break;
      default:
        resolve_aabb(collision);
        break;
    }
  }

  void World::resolve_aabb(const World::Collision &collision)
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

  void World::UpdateMovement(float dt) const {
    for (auto& body: this->Bodies)
    {
      if (!body->IsStatic)
      {
        if (body->MovingTop())
        {
          body->Position.y -= TILE_DIMENSIONS;
        }
        else if (body->MovingRight())
        {
          body->Position.x += TILE_DIMENSIONS;
        }
        else if (body->MovingBottom())
        {
          body->Position.y += TILE_DIMENSIONS;
        }
        else if (body->MovingLeft())
        {
          body->Position.x -= TILE_DIMENSIONS;
        }
      }

      body->ResetMovement();
    }
  }

}
}
