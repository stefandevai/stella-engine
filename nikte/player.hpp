#pragma once

#include <stella/components/body2d.h>
#include <stella/components/animation.h>
#include <entt/entity/registry.hpp>

namespace stella
{
namespace graphics
{
  class Display;

}
} // namespace stella

class Player
{
public:
  entt::registry& m_registry;
  enum State
  {
    IDLE,
    WALKING
  };
  const entt::registry::entity_type entity = m_registry.create();

private:
  stella::graphics::Display& Display;
  State current_state = IDLE;
  char direction      = 'd'; // Direction to which the player is facing (u, d, l, r)

public:
  Player (entt::registry& registry, stella::graphics::Display& display);
  void update();

private:
  void SetState (const stella::component::Body2D& body, stella::component::Animation& anims);
};
