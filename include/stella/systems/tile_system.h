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
    int m_last_camera_x = 0;
    int m_last_camera_y = 0;
    const int m_frustrum_culling_offset = 0;

  public:
    TileSystem(core::TileMap &tilemap, entt::registry::entity_type camera, entt::registry &registry)
      : m_tile_map(tilemap), m_camera(camera)
    {
      registry.on_destroy<components::TileComponent>().connect<&TileSystem::remove_tile_visibility>(this);
    }

    ~TileSystem() override { }

    void update(entt::registry &registry, const double dt) override
    {
      const auto &camera_position = registry.get<components::PositionComponent>(m_camera);
      const auto &camera_dimension = registry.get<components::DimensionComponent>(m_camera);

      if (m_first_position_check)
      {
        m_last_camera_x = camera_position.x;
        m_last_camera_y = camera_position.y;
        m_first_position_check = false;
      }

      if (m_last_camera_x != camera_position.x || m_last_camera_y != camera_position.y)
      {
        m_tile_map.create_tile_entities(camera_position.x + m_tile_dimension*m_frustrum_culling_offset, camera_position.x + camera_dimension.w + m_tile_dimension - m_tile_dimension*m_frustrum_culling_offset, camera_position.y + m_tile_dimension*m_frustrum_culling_offset, camera_position.y + camera_dimension.h + m_tile_dimension - m_tile_dimension*m_frustrum_culling_offset);
        m_last_camera_x = camera_position.x;
        m_last_camera_y = camera_position.y;
      }

      registry.group<components::TileComponent>(entt::get<components::PositionComponent, components::DimensionComponent>).each([this, &registry, &camera_position, &camera_dimension](auto entity, auto &tile, auto &pos, auto &dim)
      {
        // Fix to weird bug where camera values suddenly get messed up
        // I still don't understand why this happens.
        auto camera_pos = camera_position;
        auto camera_dim = camera_dimension;
        if (camera_dim.w < 1 || camera_dim.h < 1 || camera_pos.x < 0 || camera_pos.y < 0)
        {
          std::cout << "Camera values messed up. Requesting its components again.\n";
          camera_pos = registry.get<components::PositionComponent>(m_camera);
          camera_dim = registry.get<components::DimensionComponent>(m_camera);
        }

        // Destroy tile entityt if it is outside of the camera view (frustrum culling)
        // Is hidden on the left
        if (pos.x + dim.w < camera_pos.x + dim.w*m_frustrum_culling_offset)
        {
          if (registry.valid(entity))
          {
            registry.destroy(entity);
          }
        }
        // Is hidden on the right
        else if (pos.x > camera_pos.x + camera_dim.w - dim.w*m_frustrum_culling_offset)
        {
          if (registry.valid(entity))
          {
            registry.destroy(entity);
          }
        }
        // Is hidden on the top
        else if (pos.y + dim.h < camera_pos.y + dim.h*m_frustrum_culling_offset)
        {
          if (registry.valid(entity))
          {
            registry.destroy(entity);
          }
        }
        // Is hidden on the bottom
        else if (pos.y > camera_pos.y + camera_dim.h - dim.h*m_frustrum_culling_offset)
        {
          if (registry.valid(entity))
          {
            registry.destroy(entity);
          }
        }
      });
    }

    void remove_tile_visibility(entt::registry &registry, entt::entity entity)
    {
      const auto& tile = registry.get<components::TileComponent>(entity);
      const auto& pos = registry.get<components::PositionComponent>(entity);
      if (tile.collidable)
      {
        m_tile_map.collision_layers[tile.layer_id]->set_visibility(pos.x/m_tile_dimension, pos.y/m_tile_dimension, false);
      }
      else
      {
        m_tile_map.tile_layers[tile.layer_id]->set_visibility(pos.x/m_tile_dimension, pos.y/m_tile_dimension, false);
      }
    }
};
} // namespace systems
} // namespace stella


