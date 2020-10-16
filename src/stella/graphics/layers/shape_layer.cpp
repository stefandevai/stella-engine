#include "stella/graphics/layers/shape_layer.hpp"
#include "stella/components/shape.hpp"
#include "stella/components/position.hpp"
#include "stella/graphics/shader.hpp"

#include <iostream>
#include <glm/glm.hpp>                  // IWYU pragma: export
#include <glm/gtc/matrix_transform.hpp> // IWYU pragma: export

namespace stella
{
namespace graphics
{
  ShapeLayerT::ShapeLayerT (entt::registry& registry,
                            const std::string& vert_shader_path,
                            const std::string& frag_shader_path,
                            const bool fixed)
    : LayerT (registry, fixed), shader (std::make_shared<Shader> (vert_shader_path.c_str(), frag_shader_path.c_str()))
  {
    // GLint tex_ids[21] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};

    auto projection = glm::ortho (0.f, 896.f, 504.f, 0.0f, -200.f, 200.f);
    // projection = glm::translate (projection, glm::vec3 (glm::vec3 (0.f, 504.f, 0.f)));
    // projection = glm::scale(projection, glm::vec3(1.f, 1.41421356237f, 1.f));
    // projection = glm::scale(projection, glm::vec3(1.f, 1.f, 1.f));
    // projection = glm::translate (projection, glm::vec3 (0.f, -504.f, 0.f));

    shader->Enable();
    shader->SetMat4 ("proj", projection);
    // shader->SetIntv ("textures", tex_ids, 21);
    shader->Disable();
  }

  ShapeLayerT::~ShapeLayerT() { m_entities.clear(); }

  void ShapeLayerT::add (entt::entity entity) { m_entities.push_back (entity); }

  void ShapeLayerT::remove (entt::entity entity)
  {
    auto it = std::find (m_entities.begin(), m_entities.end(), entity);
    if (it != m_entities.end())
    {
      m_entities.erase (it);
    }
  }

  void ShapeLayerT::render (entt::registry& registry)
  {
    shader->Enable();
    if (!fixed)
    {
      shader->SetMat4 ("view", m_view_matrix);
    }
    m_renderer.begin();
    for (auto entity : m_entities)
    {
      if (registry.has<component::Shape> (entity) && registry.has<component::Position> (entity))
      {
        m_renderer.submit (registry, entity);
      }
    }
    m_renderer.end();
    m_renderer.draw();
  }
} // namespace graphics
} // namespace stella