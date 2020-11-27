#pragma once

#include "stella/systems/system.hpp"
#include "stella/core/resource/asset_manager.hpp"
#include "stella/graphics/renderer/mesh.hpp"
#include "stella/graphics/renderer/renderer.hpp"

namespace stella
{
namespace core
{
  class ModelAsset;
}
namespace graphics
{
  class ShaderProgram;
  class Camera;
} // namespace graphics
} // namespace stella

namespace stella
{
namespace system
{
  class Render : public System
  {
  public:
    Render (core::AssetManager& asset_manager);

    void render (entt::registry& registry, const graphics::Camera& camera, const double dt);

  private:
    core::AssetManager& m_asset_manager;
    graphics::Renderer m_renderer{m_asset_manager};
    std::shared_ptr<graphics::ShaderProgram> m_shader_program;
    std::shared_ptr<core::ModelAsset> m_model;
  };

} // namespace system
} // namespace stella
