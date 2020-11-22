#include "stella/systems/render2.hpp"
#include "stella/graphics/opengl.hpp"
#include "stella/graphics/shader_program.hpp"
#include "stella/graphics/texture2.hpp"
#include <spdlog/spdlog.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace stella
{
namespace system
{
  const float vertices[20] = {
      // positions          // texture coords
       0.5f,  0.5f, 0.0f,   1.0f, 1.0f, // top right
       0.5f, -0.5f, 0.0f,   1.0f, 0.0f, // bottom right
      -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, // bottom left
      -0.5f,  0.5f, 0.0f,   0.0f, 1.0f  // top left 
  };

  const unsigned int indices[6] = {  
      0, 1, 3, // first triangle
      1, 2, 3  // second triangle
  };

  Render::Render (core::AssetManager& asset_manager)
    : m_asset_manager (asset_manager)
  {
    //m_shader_program = std::dynamic_pointer_cast<graphics::ShaderProgram>(m_asset_manager.get("simple-shader"));
    //m_texture = std::dynamic_pointer_cast<graphics::Texture>(m_asset_manager.get("nikte"));
    m_shader_program = m_asset_manager.get<graphics::ShaderProgram>("simple-shader");
    m_texture = m_asset_manager.get<graphics::Texture>("nikte");

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
  }

  Render::~Render()
  {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
  }

  void Render::render (entt::registry& registry, const double dt)
  {
    m_texture->bind();

    glm::mat4 transform = glm::mat4(1.0f);
    transform = glm::translate(transform, glm::vec3(0.5f, -0.5f, 0.0f));
    transform = glm::rotate(transform, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    m_shader_program->set_mat_4("transform", transform);

    m_shader_program->use();
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  }

  void Render::update (entt::registry& registry, const double dt)
  {
  }

}
}
