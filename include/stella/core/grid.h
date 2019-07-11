#pragma once

#include <vector>
#include <cmath>
#include <iostream>

namespace stella
{
namespace core
{

  class Grid
  {
    protected:
      std::vector<int> m_grid;
      unsigned m_width, m_height;

    public:
      Grid(std::vector<int> grid, unsigned width, unsigned height = 0)
        : m_grid(grid), m_width(width), m_height(height)
      {
        if (m_height == 0 && m_width != 0)
        {
          m_height = ceil(grid.size() / m_width);
        }
      }
      Grid(unsigned width, unsigned height)
        : m_grid(std::vector<int>(width*height)), m_width(width), m_height(height)
      { }
      ~Grid() {}

      inline void set(std::vector<int> grid)
      {
        m_grid = grid;
      }

      inline void set_value(unsigned x, unsigned y, int value)
      {
        if (x >= 0 && x < m_width && y >= 0 && y < m_height)
        {
          m_grid[x + y*m_width] = value;
        }
        else
        {
          std::cout << "Trying to set grid value out of bounds.\n";
        }
      }

      inline std::vector<int> &get()
      {
        return m_grid;
      }

      inline int get_value(int x, int y)
      {
        if (x < 0 || static_cast<unsigned>(x) >= m_width || y < 0 || static_cast<unsigned>(y) >= m_height)
        {
          return -1;
        }
        else
        {
          return m_grid[x + y*m_width];
        }
      }
  };

} // namespace core
} // namespace stella
