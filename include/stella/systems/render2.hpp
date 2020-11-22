#pragma once

#include "./system.hpp"
#include "stella/core/asset_manager.hpp"

namespace stella
{
namespace graphics
{
  class ShaderProgram;
  class Texture;
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

    void update (entt::registry& registry, const double dt);
    void render (entt::registry& registry, const double dt);

  private:
    core::AssetManager& m_asset_manager;

    unsigned int VBO, VAO, EBO;
    std::shared_ptr<graphics::ShaderProgram> m_shader_program;
    std::shared_ptr<graphics::Texture> m_texture;

  };

}
}
