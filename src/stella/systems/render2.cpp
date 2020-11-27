#include "stella/systems/render2.hpp"
#include "stella/graphics/camera.hpp"
#include "stella/components/mesh.hpp"
#include "stella/components/sprite2.hpp"
#include <entt/entity/registry.hpp>

namespace stella
{
namespace system
{
  Render::Render (core::AssetManager& asset_manager)
    : System ("render"), m_asset_manager (asset_manager)
  { }

  void Render::render (entt::registry& registry, const graphics::Camera& camera, const double dt)
  {
    auto mesh_view = registry.view<component::Mesh>();

    for (auto entity : mesh_view)
    {
      m_renderer.add_renderable3d (registry, entity);
    }

    m_renderer.batch_sprites (registry);

    m_renderer.render(camera);
  }

}
}
