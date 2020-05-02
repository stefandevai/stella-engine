#pragma once

#include <entt/entity/entity.hpp>
#include <entt/entity/registry.hpp>
#include <set>

namespace stella
{
namespace graphics
{
  class LayerT
  {
    public:
      LayerT();
      ~LayerT();
      void add (entt::entity entity);
      void remove (entt::entity entity);
      virtual void render(entt::registry& registry) = 0;

    protected:
      std::multiset<entt::entity> m_entities;
  };
}
}