#include "stella/systems/render2.hpp"
#include "stella/graphics/opengl.hpp"
#include "stella/graphics/shader_program.hpp"
#include <spdlog/spdlog.h>

namespace stella
{
namespace system
{
  Render::Render (core::AssetManager& asset_manager)
    : m_asset_manager (asset_manager)
  {
    m_shader_program = std::dynamic_pointer_cast<graphics::ShaderProgram>(m_asset_manager.get("simple-shader"));
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0);
  }

  Render::~Render()
  {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
  }

  void Render::render (entt::registry& registry, const double dt)
  {
    m_shader_program->use();
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
  }

  void Render::update (entt::registry& registry, const double dt)
  {
  }

}
}
