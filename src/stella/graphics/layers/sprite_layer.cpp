#include "stella/graphics/layers/sprite_layer.hpp"
#include "stella/components/sprite.hpp"
#include "stella/components/position.hpp"
#include "stella/graphics/shader.hpp"

#include <glm/glm.hpp>                  // IWYU pragma: export
#include <glm/gtc/matrix_transform.hpp> // IWYU pragma: export

namespace stella
{
namespace graphics
{
  SpriteLayerT::SpriteLayerT (entt::registry& registry,
                              const std::string& vert_shader_path,
                              const std::string& frag_shader_path,
                              const bool fixed)
    : LayerT (registry, fixed), shader (std::make_shared<Shader> (vert_shader_path.c_str(), frag_shader_path.c_str()))
  {
    GLint tex_ids[21] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
    shader->Enable();
    const auto projection = glm::ortho (0.0f, 896.f, 504.f, 0.0f, -100.0f, 100.0f);
    shader->SetMat4 ("proj", projection);
    shader->SetIntv ("textures", tex_ids, 21);
    shader->Disable();
  }

  SpriteLayerT::~SpriteLayerT() { m_entities.clear(); }

  void SpriteLayerT::add (entt::entity entity) { m_entities.insert (entity); }

  void SpriteLayerT::remove (entt::entity entity)
  {
    auto it = std::find (m_entities.begin(), m_entities.end(), entity);
    if (it != m_entities.end())
    {
      m_entities.erase (it);
    }
  }

  void SpriteLayerT::render (entt::registry& registry)
  {
    shader->Enable();
    if (!fixed)
    {
      shader->SetMat4 ("view", m_view_matrix);
    }
    m_renderer.begin();
    for (auto entity : m_entities)
    {
      if (registry.has<component::SpriteT> (entity) && registry.has<component::Position> (entity))
      {
        m_renderer.submit (registry, entity);
      }
    }
    m_renderer.end();
    m_renderer.draw();
  }
} // namespace graphics
} // namespace stella
