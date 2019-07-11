#pragma once

#include <vector>
#include <cmath>
#include <iostream>

#include "./grid.h"

namespace stella
{
namespace core
{

  class MapGrid : public Grid
  {
    private:
      std::string m_render_layer_name = "basic";
      std::string m_texture_name = "";
      bool m_is_collision_grid = false;

    public:
      MapGrid(unsigned width, unsigned height)
        : Grid(width, height)
      { }
      ~MapGrid() {}

      inline void set_texture_name(const std::string &name)
      {
        m_texture_name = name;
      }

      inline void set_render_layer_name(const std::string &name)
      {
        m_render_layer_name = name;
      }

      inline void set_collision(const bool is_collision_grid)
      {
        m_is_collision_grid = is_collision_grid;
      }

      inline const std::string get_texture_name(const std::string &name)
      {
        return m_texture_name;
      }

      inline const std::string get_render_layer_name(const std::string &name)
      {
        return m_render_layer_name;
      }

      inline const bool get_collision(const bool is_collision_grid)
      {
        return m_is_collision_grid;
      }
  };

} // namespace core
} // namespace stella

