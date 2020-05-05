#include "stella/systems/group.hpp"
#include "stella/components/group.hpp"
#include "stella/components/position.hpp"
#include "stella/components/dimension.hpp"
#include <entt/entity/observer.hpp>

// TEMP
#include <iostream>
// TEMP

namespace stella
{
namespace system
{

  Group::Group (entt::registry& registry)
  {
    m_group_observer = std::make_shared<entt::observer>(registry, entt::collector.replace<component::Group>().where<component::Position>());
    m_pos_observer = std::make_shared<entt::observer>(registry, entt::collector.replace<component::Position>().where<component::Group>());
  }

  void Group::update (entt::registry& registry, const double dt)
  {
    for (const auto entity : *m_pos_observer)
    {
      auto& group = registry.get<component::Group>(entity);
      auto& pos = registry.get<component::Position>(entity);

      const auto diffx = pos.x - pos.last_x;
      const auto diffy = pos.y - pos.last_y;
      const auto diffz = pos.z - pos.last_z;

      for (auto child : group.children)
      {
        if (registry.has<component::Position>(child))
        {
          registry.patch<component::Position>(child, [&diffx, &diffy, &diffz] (auto& child_pos)
          {
            child_pos.x += diffx;
            child_pos.y += diffy;
            child_pos.z += diffz;
          });
        }
      }

      pos.last_x = pos.x;
      pos.last_y = pos.y;
      pos.last_z = pos.z;
    }
    m_pos_observer->clear();

    for (const auto entity : *m_group_observer)
    {
      auto& group = registry.get<component::Group>(entity);
      auto& pos = registry.get<component::Position>(entity);

      // Calculate new dimensions
      if (!group.children.empty())
      {
        float minx, miny, minz, maxx, maxy, maxz;
        minx = miny = minz = maxx = maxy = maxz = 0.f;

        const auto& first_child = group.children.front();
        if (registry.has<component::Position>(first_child))
        {
          const auto& first_pos = registry.get<component::Position>(first_child);
          minx = maxx = first_pos.x;
          miny = maxy = first_pos.y;
          minz = maxz = first_pos.z;
        }

        if (registry.has<component::Dimension>(first_child))
        {
          const auto& first_dim = registry.get<component::Dimension>(first_child);
          maxx += first_dim.w;
          maxy += first_dim.h;
          maxz += first_dim.d;
        }

        if (group.children.size() > 1)
        {
          for (auto child = group.children.begin() + 1; child != group.children.end(); ++child)
          {
            if (registry.has<component::Position>(*child))
            {
              const auto& child_pos = registry.get<component::Position>(*child);
              if (child_pos.x > maxx) maxx = child_pos.x;
              else if (child_pos.x < minx) minx = child_pos.x;

              if (child_pos.y > maxy) maxy = child_pos.y;
              else if (child_pos.y < miny) miny = child_pos.y;

              if (child_pos.z > maxz) maxz = child_pos.z;
              else if (child_pos.z < minz) minz = child_pos.z;
            }

            if (registry.has<component::Dimension>(first_child))
            {
              component::Position child_pos;
              if (registry.has<component::Position>(*child))
              {
                child_pos = registry.get<component::Position>(*child);
              }

              const auto& child_dim = registry.get<component::Dimension>(*child);
              if (child_pos.x + child_dim.w > maxx) maxx = child_pos.x + child_dim.w;
              if (child_pos.y + child_dim.h > maxy) maxy = child_pos.y + child_dim.h;
              if (child_pos.z + child_dim.d > maxz) maxz = child_pos.z + child_dim.d;
            }
          }
        }

        group.minx = minx;
        group.maxx = maxx;
        group.miny = miny;
        group.maxy = maxy;
        group.minz = minz;
        group.maxz = maxz;
        std::cout << minx << " " << maxx << '\n';
        std::cout << miny << " " << maxy << '\n';
        std::cout << "------------------------\n";
        pos.x = minx;
        pos.y = miny;
        pos.z = minz;
        registry.emplace_or_replace<component::Dimension>(entity, maxx - minx, maxy - miny, maxz - minz);
      }
    }
    m_group_observer->clear();
  }

} // namespace 
} // namespace stella

