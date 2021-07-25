#pragma once

#include "stella/graphics/renderer/batch2d.hpp"
#include <entt/entity/entity.hpp>
#include <entt/entity/registry.hpp>

namespace stella::core
{
class AssetManager;
}

namespace stella::graphics
{
class Mesh;
class ShaderProgram;
class Camera;

class Renderer
{
public:
  Renderer (core::AssetManager& asset_manager);
  void batch_sprites (entt::registry& registry);
  void add_renderable3d (entt::registry& registry, entt::entity entity);
  void render (const graphics::Camera& camera);

private:
  enum class ShaderType
  {
    WORLD2D,
    WORLD3D,
    GUI,
  };
  using ShaderMap = std::unordered_map<ShaderType, std::shared_ptr<ShaderProgram>>;

  core::AssetManager& m_asset_manager;
  std::vector<std::shared_ptr<graphics::Mesh>> m_meshes;
  Batch2D m_world_batch;
  Batch2D m_text_batch;
  ShaderMap m_shaders{
      {ShaderType::WORLD2D, nullptr},
      {ShaderType::WORLD3D, nullptr},
      {ShaderType::GUI, nullptr},
  };

private:
  void m_init_assets();
};

} // namespace stella::graphics
