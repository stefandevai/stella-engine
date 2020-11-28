#include "stella/graphics/renderer/renderer.hpp"
#include "stella/graphics/renderer/config.hpp"
#include "stella/graphics/renderer/mesh.hpp"
#include "stella/graphics/shader_program.hpp"
#include "stella/graphics/camera.hpp"
#include "stella/graphics/texture2.hpp"
#include "stella/core/resource/asset_manager.hpp"
#include "stella/core/resource/shader_loader.hpp"
#include "stella/components/sprite2.hpp"
#include "stella/components/mesh.hpp"
#include "stella/components/position2.hpp"

namespace stella::graphics
{
Renderer::Renderer (core::AssetManager& asset_manager) : m_asset_manager (asset_manager) { m_init_assets(); }

void Renderer::batch_sprites (entt::registry& registry)
{
  auto sprite_view = registry.view<component::Sprite>();

  m_world_batch.init_emplacing();

  for (auto entity : sprite_view)
  {
    auto& sprite = registry.get<component::Sprite> (entity);

    // If the entity has no Position component, skip it
    if (!registry.has<component::Position2>(entity))
    {
      continue;
    }

    // Load texture if it has not been loaded
    if (sprite.texture == nullptr)
    {
      sprite.texture = m_asset_manager.get<graphics::Texture> (sprite.resource_id);
    }
    m_world_batch.emplace (registry, entity);
  }

  m_world_batch.finalize_emplacing();
}

void Renderer::add_renderable3d (entt::registry& registry, entt::entity entity)
{
  if (registry.has<component::Mesh> (entity))
  {
    auto& mesh_component = registry.get<component::Mesh> (entity);
    if (mesh_component.mesh == nullptr)
    {
      mesh_component.mesh = m_asset_manager.get<graphics::Mesh> (mesh_component.resource_id);
    }

    assert (mesh_component.mesh != nullptr);
    m_meshes.emplace_back (mesh_component.mesh);
  }
}

void Renderer::render (const graphics::Camera& camera)
{
  // Render Sprites
  auto& shader2d = m_shaders[ShaderType::WORLD2D];
  shader2d->use();

  glm::mat4 model = glm::mat4 (1.0f);
  shader2d->set_mat_4 ("mvp", camera.get_projection_matrix() * camera.get_view_matrix() * model);

  m_world_batch.render (*shader2d);

  // Render Meshes
  auto& shader3d = m_shaders[ShaderType::WORLD3D];
  shader3d->use();

  model = glm::mat4 (1.0f);
  model = glm::translate (model, glm::vec3 (440.0f, 400.0f, 0.0f));
  model = glm::scale (model, glm::vec3 (10.0f, -10.0f, 10.0f));
  model = glm::rotate (model, glm::radians (-90.0f), glm::vec3 (1.0f, 0.0f, 0.0f));
  shader3d->set_mat_4 ("mvp", camera.get_projection_matrix() * camera.get_view_matrix() * model);

  for (auto& mesh : m_meshes)
  {
    mesh->render (*shader3d);
  }
  m_meshes.clear();

  // Render GUI
  auto& shader_gui = m_shaders[ShaderType::GUI];
  shader_gui->use();
}

void Renderer::m_init_assets()
{
  m_asset_manager.add<core::ShaderLoader> (WORLD2D_SHADER_NAME, WORLD2D_SHADER_PATH);
  m_shaders[ShaderType::WORLD2D] = m_asset_manager.get<graphics::ShaderProgram> (WORLD2D_SHADER_NAME);
  assert (m_shaders[ShaderType::WORLD] != nullptr);

  m_asset_manager.add<core::ShaderLoader> (WORLD3D_SHADER_NAME, WORLD3D_SHADER_PATH);
  m_shaders[ShaderType::WORLD3D] = m_asset_manager.get<graphics::ShaderProgram> (WORLD3D_SHADER_NAME);
  assert (m_shaders[ShaderType::WORLD] != nullptr);

  m_asset_manager.add<core::ShaderLoader> (GUI_SHADER_NAME, GUI_SHADER_PATH);
  m_shaders[ShaderType::GUI] = m_asset_manager.get<graphics::ShaderProgram> (GUI_SHADER_NAME);
  assert (m_shaders[ShaderType::GUI] != nullptr);
}

} // namespace stella::graphics
