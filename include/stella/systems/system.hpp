#pragma once

#include "stella/system_tags.hpp"
#include <entt/entity/registry.hpp>

namespace stella
{
namespace system
{
  class System
  {
  public:
    virtual ~System()                                               = default;
    virtual void update (entt::registry& registry, const double dt) = 0;
    const inline std::string get_tag() const { return m_tag; }

  protected:
    System() = default;
    std::string m_tag;
  };

} // namespace system
} // namespace stella
