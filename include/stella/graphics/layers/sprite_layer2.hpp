#pragma once

#include "layer2.hpp"
#include "stella/graphics/shader.hpp"
#include "stella/graphics/sprite_renderer2.hpp"

namespace stella
{
namespace graphics
{
  class SpriteLayerT : public LayerT
  {
    public:
      std::shared_ptr<Shader> shader;

    public:
      SpriteLayerT(const std::string& vert_shader_path, const std::string& frag_shader_path);
      void render(entt::registry& registry);

    private:
      void m_init();
      SpriteRendererT m_renderer{};
  };  
}
}