#pragma once

#include <entt/entity/registry.hpp>

namespace stella
{
namespace systems
{
class System
{
  public:
    virtual ~System() = default;
    virtual void update(entt::registry &registry, const double dt) {};

  protected:
    System() = default;
};
} // namespace systems
} // namespace stella
