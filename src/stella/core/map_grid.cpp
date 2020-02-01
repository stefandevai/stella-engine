#include "stella/core/map_grid.h"
#include <cmath>
#include <vector>

namespace stella
{
namespace core
{
    void MapGrid::resize (int top, int right, int bottom, int left)
    {
      if ((top != 0 && bottom != 0) || (right != 0 && left != 0))
      {
        std::cout << "ERROR: Can't resize a grid to opposite sides at the same time.\n";
        return;
      }

      const int width       = m_width + right + left;
      const int height      = m_height + top + bottom;
      const int width_diff  = width - m_width;
      const int height_diff = height - m_height;
      const int area_diff   = width * height - m_width * m_height;

      // Increased are, resize vector
      if (area_diff > 0)
      {
        m_grid.resize (width * height);
      }

      // Decrease width from right or left
      if (width_diff < 0)
      {
        for (int j = 0; j < (int) m_height; ++j)
        {
          for (int i = 0; i < width; ++i)
          {
            // Decrease from right
            if (right < 0)
            {
              // v[i+j*width] = v[i+j*m_width]
              // m_grid[i+j*width] = std::move(m_grid[i+j*m_width]);
              m_grid[i + j * width] = m_grid[i + j * m_width];
            }
            // Decrease from left
            else if (left < 0)
            {
              // v[i+j*width] = v[i+j*m_width-width_diff]
              // m_grid[i+j*width] = std::move(m_grid[i+j*m_width-width_diff]);
              m_grid[i + j * width] = m_grid[i + j * m_width - width_diff];
            }
          }
        }
        // v = v[:width*height]
        m_grid.erase (m_grid.begin() + std::abs(width * height), m_grid.end());
      }

      // Increase width to left or right
      else if (width_diff > 0)
      {
        for (int j = height - 1; j > -1; --j)
        {
          for (int i = width - 1; i > -1; --i)
          {
            // Increate to right
            if (right > 0)
            {
              if (i + 1 > (int) m_width)
              {
                // v[i+j*width] = 0
                m_grid[i + j * width].reset();
              }
              else
              {
                // v[i+j*width] = v[i+j*m_width]
                // m_grid[i+j*width] = std::move(m_grid[i+j*m_width]);
                m_grid[i + j * width] = m_grid[i + j * m_width];
              }
            }
            // Increate to left
            else if (left > 0)
            {
              if (i < left)
              {
                // v[i+j*width] = 0
                m_grid[i + j * width].reset();
              }
              else
              {
                // v[i+j*width] = v[i+j*m_width-l]
                // m_grid[i+j*width] = std::move(i+j*m_width-left);
                m_grid[i + j * width] = m_grid[i + j * m_width - left];
              }
            }
          }
        }
      }

      // Decrease height from bottom
      if (bottom < 0)
      {
        //  v = v[:width*height]
        m_grid.erase (m_grid.begin() + width * height, m_grid.end());
      }
      // Increase height to bottom
      else if (bottom > 0)
      {
        for (int i = m_height * width; i < height * width; ++i)
        {
          // v[i] = 0
          m_grid[i].reset();
        }
      }

      // Decrease height from top
      if (top < 0)
      {
        m_grid.erase (m_grid.begin(), m_grid.begin() + std::abs(height_diff * width) + 1);
      }
      // Increase height to top
      else if (top > 0)
      {
        for (int i = width * height - 1; i > -1; --i)
        {
          if (i < width * height_diff)
          {
            // v[i] = 0
            m_grid[i].reset();
          }
          else
          {
            // v[i] = v[i-height_diff*width]
            // m_grid[i] = std::move(m_grid[i-height_diff*width]);
            m_grid[i] = m_grid[i - height_diff * width];
          }
        }
      }

      m_width  = width;
      m_height = height;
    }

} // namespace core
} // namespace stella
