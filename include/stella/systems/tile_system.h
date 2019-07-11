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
    const unsigned m_tile_dimension = m_tile_map.tile_dimension();
    bool m_first_position_check = true;
    int m_last_camera_tile_x = 0;
    int m_last_camera_tile_y = 0;

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
      const int camera_tile_x = camera_position.x / m_tile_dimension;
      const int camera_tile_y = camera_position.y / m_tile_dimension;

      if (m_first_position_check)
      {
        m_last_camera_tile_x = camera_tile_x;
        m_last_camera_tile_y = camera_tile_y;

        m_first_position_check = false;
      }

      // Checks last and current camera positions
      // to determine if we should build tile entities.
      // If the last camera's tile horizontal position is different from the current camera's tile horizontal position
      if (m_last_camera_tile_x != camera_tile_x)
      {
        // Camera is moving to the left
        if (m_last_camera_tile_x > camera_tile_x)
        {
          // Create tiles to the left
          m_tile_map.create_tile_entities(camera_position.x, camera_position.x + m_tile_dimension, camera_position.y, camera_position.y + camera_dimension.h);
        }
        // Camera is moving to the right
        else
        {
          // Create tiles to the right
          m_tile_map.create_tile_entities(camera_position.x + camera_dimension.w - m_tile_dimension, camera_position.x + camera_dimension.w + m_tile_dimension, camera_position.y, camera_position.y + camera_dimension.h);
        }

        m_last_camera_tile_x = camera_tile_x;
      }

      // If the last camera's tile vertical position is different from the current camera's tile vertical position
      if (m_last_camera_tile_y != camera_tile_y)
      {
        // Camera is moving up
        if (m_last_camera_tile_y > camera_tile_y)
        {
          // Create tiles to the top
          m_tile_map.create_tile_entities(camera_position.x, camera_position.x + camera_dimension.w, camera_position.y, camera_position.y + m_tile_dimension);
        }
        // Camera is moving down
        else
        {
          // Create tiles to the bottom
          m_tile_map.create_tile_entities(camera_position.x, camera_position.x + camera_dimension.w, camera_position.y + camera_dimension.h, camera_position.y + camera_dimension.h + m_tile_dimension);
        }

        m_last_camera_tile_y = camera_tile_y;
      }
  
      registry.group<components::TileComponent>(entt::get<components::PositionComponent, components::DimensionComponent>).each([&registry, &camera_position, &camera_dimension](auto entity, auto &tile, auto &pos, auto &dim)
      {
        // Destroy tile if offbounds and create another on the opposite side
        // Is hidden on the left
        if (pos.x + dim.w < camera_position.x)
        {
          if (registry.valid(entity))
          {
            registry.destroy(entity);
          }
        }
        // Is hidden on the right
        else if (pos.x > camera_position.x + camera_dimension.w)
        {
          if (registry.valid(entity))
          {
            registry.destroy(entity);
          }
        }
        // Is hidden on the top
        else if (pos.y + dim.h < camera_position.y)
        {
          if (registry.valid(entity))
          {
            registry.destroy(entity);
          }
        }
        // Is hidden on the bottom
        else if (pos.y > camera_position.y + camera_dimension.h)
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


