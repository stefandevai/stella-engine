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
    ||  (body->MovingRight() && check_tile_collision(bw, by))
    ||  (body->MovingBottom() && check_tile_collision(bx, bh))
    ||  (body->MovingLeft() && check_tile_collision(bx-1, by)))
    {
      body->ResetMovement();
    }
  }

  bool World::check_tile_collision(int x, int y)
  {
    for (const auto &layer : m_tile_map.collision_layers)
    {
      if (layer->get_value(x, y).value > 0) return true;
    }
    return false;
  }

  void World::UpdateMovement(float dt) const {
    for (auto& body: this->Bodies)
    {
      if (!body->IsStatic)
      {
        if (body->Transition <= 0)
        {
          if (body->MovingTop())
          {
            body->Position.y -= TILE_DIMENSIONS;
            body->Transition = body->TransitionTime;
          }
          else if (body->MovingRight())
          {
            body->Position.x += TILE_DIMENSIONS;
            body->Transition = body->TransitionTime;
          }
          else if (body->MovingBottom())
          {
            body->Position.y += TILE_DIMENSIONS;
            body->Transition = body->TransitionTime;
          }
          else if (body->MovingLeft())
          {
            body->Position.x -= TILE_DIMENSIONS;
            body->Transition = body->TransitionTime;
          }
        }
        else
        {
          body->Transition -= dt*100.f;
        }
      }

      body->ResetMovement();
    }
  }

}
}
