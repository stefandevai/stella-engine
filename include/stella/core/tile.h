#pragma once

#include <bitset>
#include <cereal/types/bitset.hpp>
#include <entt/entity/registry.hpp>

namespace stella
{
namespace core
{
  struct Tile
  {
    Tile (const int value = 0) : value (value) {}
    int x           = 0;
    int y           = 0;
    int z           = 0;
    int value       = 0;
    bool visible    = false;
    bool collidable = false;
    std::bitset<4> solid_edges;
    std::bitset<4> custom_edges;
    std::bitset<4> active_edges;
    entt::entity entity = entt::null;

    bool operator== (const Tile& other) const
    {
      if (this == &other)
      {
        return true;
      }
      if (this->x == other.x && this->y == other.y && this->z == other.z && this->value == other.value &&
          this->visible == other.visible && this->collidable == other.collidable && this->entity == other.entity &&
          this->solid_edges == other.solid_edges && this->custom_edges == other.custom_edges &&
          this->active_edges == other.active_edges)
      {
        return true;
      }
      return false;
    }

    void reset()
    {
      this->value = 0;
      this->solid_edges.reset();
      this->custom_edges.reset();
      this->active_edges.reset();
      this->collidable = false;
    }

    template<class Archive>
    void serialize (Archive& archive)
    {
      archive (x, y, z, value, collidable, solid_edges, custom_edges, active_edges);
    }
  };

} // namespace core
} // namespace stella
