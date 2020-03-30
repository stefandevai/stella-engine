#include "stella/systems/tile.hpp"
#include "stella/components/position.hpp"
#include "stella/components/dimension.hpp"
#include "stella/components/tile.hpp"
#include "stella/core/tile_map.hpp"

namespace stella
{
namespace system
{
    Tile::Tile (core::TileMap& tilemap, entt::registry::entity_type camera, entt::registry& registry)
      : m_tile_map (tilemap), m_camera (camera)
    {
      registry.on_destroy<component::Tile>().connect<&Tile::remove_tile_visibility> (this);
      m_tile_dimension = m_tile_map.tile_dimension();
      const auto& camera_position  = registry.get<component::Position> (m_camera);
      m_last_camera_x        = camera_position.x;
      m_last_camera_y        = camera_position.y;
    }

    void Tile::update (entt::registry& registry, const double dt)
    {
      const auto& camera_position  = registry.get<component::Position> (m_camera);
      const auto& camera_dimension = registry.get<component::Dimension> (m_camera);

      if (m_last_camera_x != camera_position.x || m_last_camera_y != camera_position.y)
      {
        m_tile_map.create_tile_entities (
            camera_position.x - m_tile_dimension * m_frustrum_culling_offset,
            camera_position.x + camera_dimension.w + m_tile_dimension + m_tile_dimension * m_frustrum_culling_offset,
            camera_position.y - m_tile_dimension * m_frustrum_culling_offset,
            camera_position.y + camera_dimension.h + m_tile_dimension + m_tile_dimension * m_frustrum_culling_offset);
        
        m_last_camera_x = camera_position.x;
        m_last_camera_y = camera_position.y;
      }

      registry.group<component::Tile> (entt::get<component::Position, component::Dimension>)
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

                // Destroy tile entity if it is outside of the camera view (frustrum culling)
                bool will_destroy = false;
                // Is hidden on the left
                if (pos.x + dim.w < camera_pos.x - dim.w * m_frustrum_culling_offset)
                {
                  will_destroy = true;
                }
                // Is hidden on the right
                else if (pos.x > camera_pos.x + camera_dim.w + dim.w * (m_frustrum_culling_offset+1))
                {
                  will_destroy = true;
                }
                // Is hidden on the top
                else if (pos.y + dim.h < camera_pos.y - dim.h * m_frustrum_culling_offset)
                {
                  will_destroy = true;
                }
                // Is hidden on the bottom
                else if (pos.y > camera_pos.y + camera_dim.h + dim.h * m_frustrum_culling_offset)
                {
                  will_destroy = true;
                }

                if (will_destroy)
                {
                    if (registry.valid (entity))
                    {
                        registry.destroy (entity);
                    }
                }
              });
    }

    void Tile::remove_tile_visibility (entt::registry& registry, entt::entity entity)
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
} // namespace system
} // namespace stella
