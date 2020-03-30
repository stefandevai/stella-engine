#pragma once

#include <cmath>
#include <iostream>
#include <vector>

#include <cereal/types/vector.hpp>

#include "./grid.h"
#include "./tile.h"

namespace stella
{
namespace core
{
  class MapGrid : public Grid<Tile>
  {
  private:
    std::string m_name              = "Layer";
    std::string m_render_layer_name = "basic";
    std::string m_texture_name      = "";
    bool m_is_collision_grid        = false;
    int m_id                        = -1;

  public:
    MapGrid (unsigned width, unsigned height) : Grid<Tile> (width, height) {}
    MapGrid() : Grid<Tile> (0, 0) {}
    ~MapGrid() {}

    template<class Archive>
    void serialize (Archive& archive)
    {
      archive (m_name, m_id, m_grid, m_width, m_height, m_render_layer_name, m_texture_name, m_is_collision_grid);
    }

    inline void set_name (const std::string& name) { m_name = name; }

    inline void set_texture_name (const std::string& name) { m_texture_name = name; }

    inline void set_render_layer_name (const std::string& name) { m_render_layer_name = name; }

    inline void set_visibility (const unsigned int x, const unsigned int y, const bool visibility)
    {
      if (x >= 0 && x < m_width && y >= 0 && y < m_height)
      {
        m_grid[x + y * m_width].visible = visibility;
      }
      else
      {
        // std::cout << "Trying to reach a grid value out of bounds.\n";
      }
    }

    inline void set_collision (const bool is_collision_grid) { m_is_collision_grid = is_collision_grid; }

    inline void set_entity (const unsigned int x, const unsigned int y, const entt::entity entity)
    {
      if (x >= 0 && x < m_width && y >= 0 && y < m_height)
      {
        m_grid[x + y * m_width].entity = entity;
      }
      else
      {
        // std::cout << "Trying to reach a grid value out of bounds.\n";
      }
    }

    inline void set_id (const int id) { m_id = id; }

    inline const std::string get_name() { return m_name; }

    inline const std::string get_texture_name() const { return m_texture_name; }

    inline const std::string get_render_layer_name() const { return m_render_layer_name; }

    inline const bool is_collision_grid() const { return m_is_collision_grid; }

    inline int get_id() const { return m_id; }

    void resize (int top, int right, int bottom, int left);
  };

} // namespace core
} // namespace stella
