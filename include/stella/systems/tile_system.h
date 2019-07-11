#pragma once

#include "./system.h"
#include "../components.h"
#include "../core/tile_map.h"

namespace stella
{
namespace systems
{
class TileSystem : public System
{
  private:
    core::TileMap &m_tile_map;
    entt::registry::entity_type m_camera;

  public:
    TileSystem(core::TileMap &tilemap, entt::registry::entity_type camera)
      : m_tile_map(tilemap), m_camera(camera)
    {

    }

    ~TileSystem() override { }

    void update(entt::registry &registry, const double dt) override
    {
      const auto &camera_position = registry.get<components::PositionComponent>(m_camera);
      const auto &camera_dimension = registry.get<components::DimensionComponent>(m_camera);
  
      registry.group<components::TileComponent>(entt::get<components::PositionComponent, components::DimensionComponent>).each([this, &registry, &camera_position, &camera_dimension](auto entity, auto &tile, auto &pos, auto &dim)
      {
        //// Destroy tile if offbounds and create another on the opposite side
        //// Is hidden on the left
        //if (pos.x + dim.w < camera_position.x + dim.w)
        //{
          //if (registry.valid(entity))
          //{
            //// Creates tile entity on the right
            //m_tile_map.create_tile_entity(camera_position.x + camera_dimension.w - dim.w, pos.y, tile.layer_id);
            //registry.destroy(entity);
          //}
        //}
        //// Is hidden on the right
        //else if (pos.x > camera_position.x + camera_dimension.w - dim.w)
        //{
          //if (registry.valid(entity))
          //{
            //// Creates tile entity on the left
            //m_tile_map.create_tile_entity(camera_position.x + dim.w, pos.y, tile.layer_id);
            //registry.destroy(entity);
          //}
        //}
        //// Is hidden on the top
        //else if (pos.y + dim.h < camera_position.y + dim.h)
        //{
          //if (registry.valid(entity))
          //{
            //// Creates tile entity on the bottom
            //m_tile_map.create_tile_entity(camera_position.y + camera_dimension.h - dim.h, pos.x, tile.layer_id);
            //registry.destroy(entity);
          //}
        //}
        //// Is hidden on the bottom
        //else if (pos.y > camera_position.y + camera_dimension.h - dim.h)
        //{
          //if (registry.valid(entity))
          //{
            //// Creates tile entity on the top
            //m_tile_map.create_tile_entity(camera_position.y + dim.h, pos.x, tile.layer_id);
            //registry.destroy(entity);
          //}
        //}
      });
    }
};
} // namespace systems
} // namespace stella


