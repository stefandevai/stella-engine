#pragma once

#include <cmath>
#include <iostream>
#include <vector>

namespace stella
{
namespace core
{
  template<typename T>
  class Grid
  {
  public:
    std::vector<T> m_grid;
    Grid (unsigned width, unsigned height)
      : m_grid (std::vector<T> (width * height)), m_width (width), m_height (height)
    {
    }
    ~Grid() {}

    inline unsigned width() { return m_width; }
    inline unsigned height() { return m_height; }

    inline void set_value (unsigned x, unsigned y, T value)
    {
      if (x >= 0 && x < m_width && y >= 0 && y < m_height)
      {
        m_grid[x + y * m_width] = value;
      }
      else
      {
        // std::cout << "Trying to set grid value out of bounds.\n";
      }
    }

    inline T get_value (int x, int y)
    {
      if (x < 0 || static_cast<unsigned> (x) >= m_width || y < 0 || static_cast<unsigned> (y) >= m_height)
      {
        return -1;
      }
      else
      {
        return m_grid[x + y * m_width];
      }
    }

  protected:
    unsigned m_width, m_height;
  };

} // namespace core
} // namespace stella
