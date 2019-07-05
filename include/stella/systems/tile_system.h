#pragma once

#include "./system.h"
#include "../components.h"

namespace stella
{
namespace systems
{
class TileSystem : public System
{
  private:
    entt::registry::entity_type m_camera;
  public:
    TileSystem(entt::registry::entity_type camera)
      : m_camera(camera)
    {

    }

    ~TileSystem() override { }

    void update(entt::registry &registry, const double dt) override
    {
      const auto &camera_position = registry.get<components::PositionComponent>(m_camera);
  
      registry.group<components::TileComponent>(entt::get<components::PositionComponent, components::DimensionComponent>).each([&registry, &camera_position](auto entity, auto &tile, auto &pos, auto &dim)
      {
        if (pos.x + dim.w < camera_position.x - dim.w)
        {
          if (registry.valid(entity))
          {
            registry.destroy(entity);
          }
        }
      });
    }
};
} // namespace systems
} // namespace stella


