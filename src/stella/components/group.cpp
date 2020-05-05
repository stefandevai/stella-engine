#include "stella/components/group.hpp"
#include "stella/components/in_group.hpp"

#include <algorithm>

namespace stella
{
namespace component
{

void Group::add (const entt::entity entity, entt::registry& registry)
{
    registry.emplace_or_replace<InGroup> (entity);
    children.push_back(entity);
}

void Group::remove (const entt::entity entity, entt::registry& registry)
{
  registry.remove<InGroup>(entity);
  auto it = std::find (children.begin(), children.end(), entity);
  if (it != children.end())
  {
    children.erase(it);
  }
}

} // namespace component
} // namespace stella
