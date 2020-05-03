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

      LayerT(entt::registry& registry, const bool fixed = false);
      ~LayerT();
      void add (entt::entity entity);
      void remove (entt::entity entity);
      virtual void render(entt::registry& registry) = 0;
      inline void set_view_matrix (const glm::mat4& view) { m_view_matrix = view; };
      inline const bool has (const entt::entity entity) { return m_entities.find (entity) != m_entities.end(); }

    protected:
      struct CompSpriteZ
      {
        CompSpriteZ (entt::registry& registry) : m_registry(registry) {}
        entt::registry& m_registry;
        bool operator() (const entt::entity lhs, const entt::entity rhs) const noexcept
        {
          const auto& posl = m_registry.get<component::Position>(lhs);
          const auto& posr = m_registry.get<component::Position>(rhs);
          const auto& diml = m_registry.get<component::Dimension>(lhs);
          const auto& dimr = m_registry.get<component::Dimension>(rhs);
          // If their z is equal, use y position as a second parameter of differentiation
          if (posl.z == posr.z)
          {
            return ((posl.y + diml.h) < (posr.y + dimr.h));
          }

          return (posl.z < posr.z);
        }
      };
      CompSpriteZ comparator{m_registry};

      std::multiset<entt::entity, CompSpriteZ> m_entities{comparator};
      glm::mat4 m_view_matrix;
  };
}
}