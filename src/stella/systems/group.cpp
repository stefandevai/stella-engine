#include "stella/systems/group.hpp"
#include "stella/components/group.hpp"
#include "stella/components/position.hpp"

// TEMP
#include <iostream>
// TEMP

namespace stella
{
namespace system
{

  Group::Group ()
  {

  }

  void Group::update (entt::registry& registry, const double dt)
  {
    registry.group<component::Group> (entt::get<component::Position>)
    .each([&registry] (auto entity, auto& group, auto& pos)
    {
      if (pos.has_changed())
      {
        const auto diffx = pos.x - pos.last_x;
        const auto diffy = pos.y - pos.last_y;
        const auto diffz = pos.z - pos.last_z;

        for (auto child : group.children)
        {
          if (registry.has<component::Position>(child))
          {
            auto& pos = registry.get<component::Position>(child);
            pos.x += diffx;
            pos.y += diffy;
            pos.z += diffz;
          }
        }

        pos.last_x = pos.x;
        pos.last_y = pos.y;
        pos.last_z = pos.z;
      }
    });
  }

} // namespace 
} // namespace stella

