#pragma once

#include "system.hpp"
#include <entt/entity/registry.hpp>

namespace stella
{
namespace system
{

  class Group : public System
  {
    public:
      Group ();
      void update (entt::registry& registry, const double dt) override;

    private:
  };

} // namespace 
} // namespace stella

