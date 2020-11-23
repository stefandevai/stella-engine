#pragma once

#include "stella/systems/system.hpp"
#include "stella/core/asset_manager.hpp"

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
    ~Render ();

    void render (entt::registry& registry, const graphics::Camera& camera, const double dt);

  private:
    core::AssetManager& m_asset_manager;

    unsigned int VBO, VAO;
    std::shared_ptr<graphics::ShaderProgram> m_shader_program;
    std::shared_ptr<graphics::Texture> m_texture;

  };

}
}
