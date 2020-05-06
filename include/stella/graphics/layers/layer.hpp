#pragma once

#include <entt/entity/entity.hpp>
#include <entt/entity/registry.hpp>
#include "stella/components/position.hpp"
#include "stella/components/dimension.hpp"
#include <glm/mat4x4.hpp>
#include <set>

namespace stella
{
namespace graphics
{
  class LayerT
  {
  protected:
    entt::registry& m_registry;

  public:
    bool fixed;
    LayerT (entt::registry& registry, const bool fixed = false);

    virtual void add (entt::entity entity)         = 0;
    virtual void remove (entt::entity entity)      = 0;
    virtual void render (entt::registry& registry) = 0;
    inline void set_view_matrix (const glm::mat4& view) { m_view_matrix = view; };
    virtual const bool has (const entt::entity entity) = 0;

  protected:
    glm::mat4 m_view_matrix;
  };
} // namespace graphics
} // namespace stella