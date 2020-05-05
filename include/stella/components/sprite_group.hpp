#pragma once

#include "component.hpp"
#include <vector>
#include <entt/entity/entity.hpp>
#include <entt/entity/registry.hpp>

namespace stella
{
namespace component
{
  struct SpriteGroup : public Component
  {
    SpriteGroup() : Component ("Sprite Group") {}
    SpriteGroup(const unsigned width, const unsigned height, const std::vector<entt::entity>& children)
    : Component ("Sprite Group"), width(width), height(height), children (children)
    {}
    unsigned width = 0, height = 0;
    std::vector<entt::entity> children{};

    void add (const entt::entity entity, entt::registry& registry);
    void remove (const entt::entity entity, entt::registry& registry);
  };
} // namespace component
} // namespace stella
