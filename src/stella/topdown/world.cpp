#include "stella/topdown/world.hpp"
#include "stella/topdown/body.hpp"
#include <algorithm>
#include <cmath>
#include <bitset>
#include "stella/core/map_grid.hpp"
#include "stella/core/tile.hpp"
#include "stella/core/tile_map.hpp"

namespace stella
{
namespace topdown
{
  World::World (const core::TileMap& tile_map) : m_tile_map (tile_map) {}

  World::~World() { this->Bodies.clear(); }

  void World::Update (float dt)
  {
    this->UpdateCollisions();
    this->UpdateMovement (dt);
  }

  void World::AddBody (std::shared_ptr<stella::topdown::Body> body) { this->Bodies.emplace_back (body); }

  void World::RemoveBody (std::shared_ptr<stella::topdown::Body> body)
  {
    auto it = std::find (this->Bodies.begin(), this->Bodies.end(), body);
    if (it != this->Bodies.end())
      this->Bodies.erase (it);
  }

  void World::UpdateCollisions()
  {
    for (const auto& body : this->Bodies)
    {
      body->Collisions.reset();
      if (!body->IsStatic)
      {
        resolve_collisions (body);
      }
    }
  }

  void World::resolve_collisions (const std::shared_ptr<Body>& body)
  {
    const int bx = body->Position.x / TILE_DIMENSIONS;
    const int by = body->Position.y / TILE_DIMENSIONS;
    const int bw = (body->Position.x + body->Dimension.x) / TILE_DIMENSIONS;
    const int bh = (body->Position.y + body->Dimension.y) / TILE_DIMENSIONS;

    if ((body->MovingTop() && check_tile_collision (bx, by - 1)) || (body->MovingRight() && check_tile_collision (bw, by)) ||
        (body->MovingBottom() && check_tile_collision (bx, bh)) || (body->MovingLeft() && check_tile_collision (bx - 1, by)))
    {
      body->ResetMovement();
    }
  }

  bool World::check_tile_collision (int x, int y)
  {
    for (const auto& layer : m_tile_map.layers)
    {
      if (layer->get_value (x, y).collidable)
        return true;
    }
    return false;
  }

  void World::UpdateMovement (float dt) const
  {
    for (auto& body : this->Bodies)
    {
      if (!body->IsStatic)
      {
        if (body->Transition <= 0)
        {
          if (body->state != BodyState::IDLE)
          {
            body->state = BodyState::IDLE;
          }

          // Vertical movement
          if (body->MovingTop() && !body->MovingBottom())
          {
            body->Target.y   = body->Position.y - TILE_DIMENSIONS;
            body->Transition = body->MovementDelay;
            body->state      = BodyState::MOVING;
            body->direction &= ~BodyDirection::BOTTOM;
            body->direction = BodyDirection::TOP;
          }
          else if (body->MovingBottom() && !body->MovingTop())
          {
            body->Target.y   = body->Position.y + TILE_DIMENSIONS;
            body->Transition = body->MovementDelay;
            body->state      = BodyState::MOVING;
            body->direction &= ~BodyDirection::TOP;
            body->direction = BodyDirection::BOTTOM;
          }

          // Horizontal movement
          if (body->MovingRight() && !body->MovingLeft())
          {
            body->Target.x   = body->Position.x + TILE_DIMENSIONS;
            body->Transition = body->MovementDelay;
            body->state      = BodyState::MOVING;
            body->direction &= ~BodyDirection::LEFT;
            body->direction |= BodyDirection::RIGHT;
          }
          else if (body->MovingLeft() && !body->MovingRight())
          {
            body->Target.x   = body->Position.x - TILE_DIMENSIONS;
            body->Transition = body->MovementDelay;
            body->state      = BodyState::MOVING;
            body->direction &= ~BodyDirection::RIGHT;
            body->direction |= BodyDirection::LEFT;
          }

          body->LastPosition   = body->Position;
          body->LastTransition = body->Transition;
        }
        else
        {
          body->Transition -= dt * 100.f;
          if (body->Transition <= 0.f)
          {
            body->Position.x = body->Target.x;
            body->Position.y = body->Target.y;
          }
          else
          {
            if (body->state != BodyState::MOVING)
            {
              body->state = BodyState::MOVING;
            }
            const float partial_transition = 1.f - body->Transition / body->LastTransition;
            const float partial_movement   = (float) TILE_DIMENSIONS * partial_transition;
            float xsign                    = copysignf (1.0f, body->Target.x - body->Position.x);
            float ysign                    = copysignf (1.0f, body->Target.y - body->Position.y);
            if (body->Target.y == body->Position.y)
              ysign = 0.f;
            if (body->Target.x == body->Position.x)
              xsign = 0.f;

            body->Position.x = body->LastPosition.x + partial_movement * xsign;
            body->Position.y = body->LastPosition.y + partial_movement * ysign;
          }
        }
      }

      body->ResetMovement();
    }
  }
} // namespace topdown
} // namespace stella
