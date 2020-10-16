#pragma once

#include "component.hpp"
#include <entt/entity/entity.hpp>
#include <vector>

namespace stella
{
namespace component
{
  struct Block : public Component
  {
    Block (const int w, const int h, const int d) : Component ("Block"), w (w), h (h), d (d) {}
    int w, h, d;
    std::vector<entt::entity> entities;
  };

} // namespace component
} // namespace stella
