#pragma once

#include <memory>
#include <vector>
#include <algorithm>

#include "stella/graphics/layers/layer.hpp"
#include "stella/graphics/shape.hpp"
#include "stella/graphics/shape_renderer.hpp"
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
  class ShapeLayerT : public LayerT
  {
  private:
    std::vector<entt::entity> m_entities;
    ShapeRendererT m_renderer{};

  public:
    std::shared_ptr<Shader> shader;

  public:
    ShapeLayerT (entt::registry& registry,
                 const std::string& vert_shader_path,
                 const std::string& frag_shader_path,
                 const bool fixed = false);
    virtual ~ShapeLayerT();
    void add (entt::entity entity);
    void remove (entt::entity entity);
    void render (entt::registry& registry);
    inline const bool has (const entt::entity entity)
    {
      return std::find (m_entities.begin(), m_entities.end(), entity) != m_entities.end();
    }
  };
} // namespace graphics
} // namespace stella
