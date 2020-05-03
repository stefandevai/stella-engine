#include "stella/graphics/layers/sprite_layer2.hpp"
#include "stella/components/sprite2.hpp"
#include "stella/components/position.hpp"

#include <iostream>
#include <glm/glm.hpp>                  // IWYU pragma: export
#include <glm/gtc/matrix_transform.hpp> // IWYU pragma: export

namespace stella
{
namespace graphics
{
  SpriteLayerT::SpriteLayerT(const std::string& vert_shader_path, const std::string& frag_shader_path, const bool fixed)
  : LayerT(fixed), shader(std::make_shared<Shader>(vert_shader_path.c_str(), frag_shader_path.c_str()))
  {
    GLint tex_ids[21] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
    shader->Enable();
    const auto projection = glm::ortho (0.0f, 896.f, 504.f, 0.0f, -20.0f, 0.0f);
    shader->SetMat4 ("proj", projection);
    shader->SetIntv ("textures", tex_ids, 21);
    shader->Disable();
  }

  void SpriteLayerT::render(entt::registry& registry)
  {
      shader->Enable();
      if (!fixed)
      {
        shader->SetMat4 ("view", m_view_matrix);
      }
      m_renderer.begin();
      for (auto entity : m_entities)
      {
          if (registry.has<component::SpriteT>(entity) && registry.has<component::Position>(entity))
          {
            m_renderer.submit(registry, entity);
          }
      }
      m_renderer.end();
      m_renderer.draw();
  }
}
}