#pragma once

#include "component.hpp"
#include <vector>
#include <entt/entity/entity.hpp>
#include <entt/entity/registry.hpp>

namespace stella
{
namespace component
{
  struct Group : public Component
  {
    Group() : Component ("Group") {}
    Group (const unsigned width, const unsigned height, const std::vector<entt::entity>& children) : Component ("Group"), width (width), height (height), children (children) {}
    void add (const entt::entity entity, entt::registry& registry);
    void remove (const entt::entity entity, entt::registry& registry);

    unsigned width = 0, height = 0;
    int tile_width = 0, tile_height = 0;
    float minx = 0.f, miny = 0.f, minz = 0.f, maxx = 0.f, maxy = 0.f, maxz = 0.f;

    std::vector<entt::entity> children{};
    std::string texture = "";

    struct update_event
    {
    };
  };
} // namespace component
} // namespace stella
