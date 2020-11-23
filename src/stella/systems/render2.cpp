#include "stella/systems/render2.hpp"
#include "stella/graphics/opengl.hpp"
#include "stella/graphics/shader_program.hpp"
#include "stella/graphics/texture2.hpp"
#include "stella/graphics/camera.hpp"
#include <spdlog/spdlog.h>
#include <entt/entity/registry.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace stella
{
namespace system
{

  const float vertices[180] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
  };

  Render::Render (core::AssetManager& asset_manager)
    : System ("render"), m_asset_manager (asset_manager)
  {
    m_shader_program = m_asset_manager.get<graphics::ShaderProgram>("simple-shader");
    m_texture = m_asset_manager.get<graphics::Texture>("crate");

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    m_shader_program->use();
    glm::mat4 projection = glm::ortho(0.0f, 1792.0f, 1008.0f, 0.0f, 0.1f, 1000.0f);
    m_shader_program->set_mat_4("projection", projection);
  }

  Render::~Render()
  {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
  }

  float accumulator = 0.0f;
  void Render::render (entt::registry& registry, const graphics::Camera& camera, const double dt)
  {
    m_texture->bind();
    accumulator += static_cast<float>(dt);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(200.0f, 200.0f, 200.0f));
    model = glm::rotate(model, accumulator, glm::vec3(0.9f, 0.6f, 0.3f));

    //glm::mat4 view;
    //view = glm::lookAt(glm::vec3(-400.0f, -200.0f, 300.0f), 
                       //glm::vec3(-400.0f, -200.0f, 0.0f), 
                       //glm::vec3(0.0f, 1.0f, 0.0f));

    //glm::mat4 view = glm::mat4(1.0f);
    //view = glm::translate(view, glm::vec3(400.0f, 200.0f, -300.0f));
    //view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

    m_shader_program->set_mat_4("model", model);
    m_shader_program->set_mat_4("view", camera.get_view_matrix());

    m_shader_program->use();
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
  }

}
}
