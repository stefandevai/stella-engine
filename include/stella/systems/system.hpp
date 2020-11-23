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
    System (const std::string& tag) : m_tag (tag) {}
    virtual ~System()                                               = default;
    //virtual void update (entt::registry& registry, const double dt) = 0;
    //virtual void render (entt::registry& registry, const double dt) {}
    const inline std::string get_tag() const { return m_tag; }

  protected:
    //System() = default;
    const std::string m_tag;
  };

} // namespace system
} // namespace stella
