#pragma once

#include <entt/entity/entity.hpp>
#include <entt/entity/registry.hpp>
#include <glm/mat4x4.hpp>
#include <set>

namespace stella
{
namespace graphics
{
  class LayerT
  {
    public:
      bool fixed;

      LayerT(const bool fixed = false);
      ~LayerT();
      void add (entt::entity entity);
      void remove (entt::entity entity);
      virtual void render(entt::registry& registry) = 0;
      inline void set_view_matrix (const glm::mat4& view) { m_view_matrix = view; };
      inline const bool has (const entt::entity entity) { return m_entities.find (entity) != m_entities.end(); }

    protected:
      std::multiset<entt::entity> m_entities;
      glm::mat4 m_view_matrix;
  };
}
}