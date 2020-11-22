#pragma once

#include "./system.hpp"
#include "stella/core/asset_manager.hpp"

namespace stella
{
namespace graphics
{
  class ShaderProgram;
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

    unsigned int VBO, VAO;
    std::shared_ptr<graphics::ShaderProgram> m_shader_program;
    //int shaderProgram;
    float vertices[9] =
    {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f,
    }; 

  };

}
}
