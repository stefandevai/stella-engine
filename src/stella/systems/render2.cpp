#include "stella/systems/render2.hpp"
#include "stella/graphics/opengl.hpp"
#include "stella/graphics/shader_program.hpp"
#include "stella/graphics/camera.hpp"
#include "stella/core/model_loader.hpp"
#include <spdlog/spdlog.h>
#include <entt/entity/registry.hpp>

namespace stella
{
namespace system
{
  Render::Render (core::AssetManager& asset_manager)
    : System ("render"), m_asset_manager (asset_manager)
  {
    m_asset_manager.add <core::ModelLoader> ("skull", "/Users/stefandevai/Developer/games/stella-engine/nikte2/assets/models/skull/12140_Skull_v3_L2.obj", &m_asset_manager);
    m_model = m_asset_manager.get <core::ModelAsset> ("skull");
    m_shader_program = m_asset_manager.get<graphics::ShaderProgram>("simple-shader");
  }

  float accumulator = 0.0f;
  void Render::render (entt::registry& registry, const graphics::Camera& camera, const double dt)
  {
    accumulator += static_cast<float>(dt);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
    model = glm::rotate(model, accumulator, glm::vec3(0.9f, 0.0f, 0.0f));

    for (auto& mesh : m_model->meshes)
    {
      mesh->draw(*m_shader_program);
    }

    m_shader_program->set_mat_4("mvp", camera.get_projection_matrix() * camera.get_view_matrix() * model);
    m_shader_program->use();

  }

}
}
