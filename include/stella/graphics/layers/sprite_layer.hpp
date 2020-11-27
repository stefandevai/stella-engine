#pragma once

#include "layer.hpp"
#include "stella/graphics/sprite_renderer.hpp"
namespace stella
{
namespace graphics
{
  class Shader;
}
} // namespace stella

namespace stella
{
namespace graphics
{
  class SpriteLayerT : public LayerT
  {
  public:
    std::shared_ptr<Shader> shader;

  public:
    SpriteLayerT (entt::registry& registry, const std::string& vert_shader_path, const std::string& frag_shader_path, const bool fixed = false);
    virtual ~SpriteLayerT();
    void add (entt::entity entity);
    void remove (entt::entity entity);
    void render (entt::registry& registry);
    inline const bool has (const entt::entity entity) { return m_entities.find (entity) != m_entities.end(); }

  private:
    struct CompSpriteZ
    {
      CompSpriteZ (entt::registry& registry) : m_registry (registry) {}
      entt::registry& m_registry;
      bool operator() (const entt::entity lhs, const entt::entity rhs) const noexcept
      {
        const auto& posl = m_registry.get<component::Position> (lhs);
        const auto& posr = m_registry.get<component::Position> (rhs);
        const auto& diml = m_registry.get<component::Dimension> (lhs);
        const auto& dimr = m_registry.get<component::Dimension> (rhs);
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

    void m_init();
    SpriteRendererT m_renderer{};
  };
} // namespace graphics
} // namespace stella
