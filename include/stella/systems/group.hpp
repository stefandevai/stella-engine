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
      Group (entt::registry& registry);
      void update (entt::registry& registry, const double dt) override;

    private:
      std::shared_ptr<entt::observer> m_group_observer;
      std::shared_ptr<entt::observer> m_pos_observer;
  };

} // namespace 
} // namespace stella

