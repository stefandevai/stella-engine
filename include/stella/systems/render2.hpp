#pragma once

#include "stella/systems/system.hpp"
#include "stella/core/asset_manager.hpp"
#include "stella/graphics/mesh.hpp"

namespace stella
{
namespace graphics
{
  class ShaderProgram;
  class Texture;
  class Camera;
}
}

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

    std::shared_ptr<graphics::ShaderProgram> m_shader_program;
    std::vector<std::shared_ptr<graphics::Mesh>> meshes;

  };

}
}
