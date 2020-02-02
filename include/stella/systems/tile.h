#pragma once

#include "../components.h"
#include "../core/tile_map.h"
#include "./system.h"

namespace stella
{
namespace system
{
  class Tile : public System
  {
  private:
    core::TileMap& m_tile_map;
    entt::registry::entity_type m_camera;
    const unsigned m_tile_dimension     = m_tile_map.tile_dimension();
    bool m_first_position_check         = true;
    int m_last_camera_x                 = 0;
    int m_last_camera_y                 = 0;
    const int m_frustrum_culling_offset = 0;

  public:
    Tile (core::TileMap& tilemap, entt::registry::entity_type camera, entt::registry& registry)
      : m_tile_map (tilemap), m_camera (camera)
    {
      registry.on_destroy<component::Tile>().connect<&Tile::remove_tile_visibility> (this);
    }

    ~Tile() override {}

    void update (entt::registry& registry, const double dt) override
    {
      const auto& camera_position  = registry.get<component::Position> (m_camera);
      const auto& camera_dimension = registry.get<component::Dimension> (m_camera);

      if (m_first_position_check)
      {
        m_last_camera_x        = camera_position.x;
        m_last_camera_y        = camera_position.y;
        m_first_position_check = false;
      }

      if (m_last_camera_x != camera_position.x || m_last_camera_y != camera_position.y)
      {
        m_tile_map.create_tile_entities (
            camera_position.x + m_tile_dimension * m_frustrum_culling_offset,
            camera_position.x + camera_dimension.w + m_tile_dimension - m_tile_dimension * m_frustrum_culling_offset,
            camera_position.y + m_tile_dimension * m_frustrum_culling_offset,
            camera_position.y + camera_dimension.h + m_tile_dimension - m_tile_dimension * m_frustrum_culling_offset);
        m_last_camera_x = camera_position.x;
        m_last_camera_y = camera_position.y;
      }

      registry
          .group<component::Tile> (entt::get<component::Position, component::Dimension>)
          .each (
              [this, &registry, &camera_position, &camera_dimension] (auto entity, auto& tile, auto& pos, auto& dim) {
                // Fix to weird bug where camera values suddenly get messed up
                // I still don't understand why this happens.
                auto camera_pos = camera_position;
                auto camera_dim = camera_dimension;

                // if (camera_dim.w < 1 || camera_dim.h < 1 || camera_pos.x < 0 ||
                // camera_pos.y < 0)
                // {
                //   std::cout << "Camera values messed up. Requesting its
                //   components again.\n"; camera_pos =
                //   registry.get<component::Position>(m_camera);
                //   camera_dim =
                //   registry.get<component::Dimension>(m_camera);
                // }

                // Destroy tile entity if it is outside of the camera view (frustrum
                // culling) Is hidden on the left
                if (pos.x + dim.w < camera_pos.x + dim.w * m_frustrum_culling_offset)
                {
                  if (registry.valid (entity))
                  {
                    registry.destroy (entity);
                  }
                }
                // Is hidden on the right
                else if (pos.x > camera_pos.x + camera_dim.w - dim.w * m_frustrum_culling_offset)
                {
                  if (registry.valid (entity))
                  {
                    registry.destroy (entity);
                  }
                }
                // Is hidden on the top
                else if (pos.y + dim.h < camera_pos.y + dim.h * m_frustrum_culling_offset)
                {
                  if (registry.valid (entity))
                  {
                    registry.destroy (entity);
                  }
                }
                // Is hidden on the bottom
                else if (pos.y > camera_pos.y + camera_dim.h - dim.h * m_frustrum_culling_offset)
                {
                  if (registry.valid (entity))
                  {
                    registry.destroy (entity);
                  }
                }
              });
    }

    void remove_tile_visibility (entt::registry& registry, entt::entity entity)
    {
      const auto& tile = registry.get<component::Tile> (entity);
      const auto& pos  = registry.get<component::Position> (entity);
      m_tile_map.layers[tile.layer_id]->set_visibility (pos.x / m_tile_dimension, pos.y / m_tile_dimension, false);

      // if (tile.collidable)
      //{
      // m_tile_map.collision_layers[tile.layer_id]->set_visibility(pos.x/m_tile_dimension,
      // pos.y/m_tile_dimension, false);
      //}
      // else
      //{
      // m_tile_map.tile_layers[tile.layer_id]->set_visibility(pos.x/m_tile_dimension,
      // pos.y/m_tile_dimension, false);
      //}
    }
  };
} // namespace system
} // namespace stella
