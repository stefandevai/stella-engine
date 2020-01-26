#pragma once

#include <vector>
#include <cmath>
#include <iostream>

namespace stella
{
namespace core
{
  template <typename T>
  class Grid
  {
    protected:
      std::vector<T> m_grid;
      unsigned m_width, m_height;

    public:
      Grid(unsigned width, unsigned height)
        : m_grid(std::vector<T>(width*height)), m_width(width), m_height(height)
      { }
      ~Grid() {}

      inline void set_value(unsigned x, unsigned y, T value)
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

      inline T get_value(int x, int y)
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

      void resize(int top, int right, int bottom, int left)
      {
        assert ((top == 0 || bottom == 0) && "You can't resize a grid to opposite sides at the same time.");
        assert ((right == 0 || left == 0) && "You can't resize a grid to opposite sides at the same time.");
        
        const int width = m_width + right + left;
        const int height = m_height + top + bottom;
        const int width_diff = width - m_width; 
        const int height_diff = height - m_height;
        const int area_diff = width*height - m_width*m_height;

        // Increased are, resize vector
        if (area_diff > 0)
        {
          m_grid.resize(width*height);
        }

        // Decrease width from right or left
        if (width_diff < 0)
        {
          for (int j = 0; j < (int)m_height; ++j)
          {
            for (int i = 0; i < width; ++i)
            {
              // Decrease from right
              if (right < 0)
              {
                // v[i+j*width] = v[i+j*m_width]
                // m_grid[i+j*width] = std::move(m_grid[i+j*m_width]);
                m_grid[i+j*width] = m_grid[i+j*m_width];
              }
              // Decrease from left
              else if (left < 0)
              {
                // v[i+j*width] = v[i+j*m_width-width_diff]
                // m_grid[i+j*width] = std::move(m_grid[i+j*m_width-width_diff]);
                m_grid[i+j*width] = m_grid[i+j*m_width-width_diff];
              }
            }
          }
          // v = v[:width*height]
          m_grid.erase(m_grid.begin()+width*height, m_grid.end());
        }

        // Increase width to left or right
        else if (width_diff > 0)
        {
          for (int j = height-1; j > -1; --j)
          {
            for (int i = width-1; i > -i; --i)
            {
              // Increate to right
              if (right > 0)
              {
                if (i+1 > (int)m_width)
                {
                  // v[i+j*width] = 0
                  m_grid[i+j*width].reset();
                }
                else
                {
                  // v[i+j*width] = v[i+j*m_width]
                  //m_grid[i+j*width] = std::move(m_grid[i+j*m_width]);
                  m_grid[i+j*width] = m_grid[i+j*m_width];
                }
              }
              // Increate to left
              else if (left > 0)
              {
                if (i < left)
                {
                  // v[i+j*width] = 0
                  m_grid[i+j*width].reset();
                }
                else
                {
                  // v[i+j*width] = v[i+j*m_width-l]
                  //m_grid[i+j*width] = std::move(i+j*m_width-left);
                  m_grid[i+j*width] = m_grid[i+j*m_width-left];
                }
                
              }
            }
          }
        }

        // Decrease height from bottom
        if (bottom < 0)
        {
          //  v = v[:width*height]
          m_grid.erase(m_grid.begin()+width*height, m_grid.end());
        }
        // Increase height to bottom
        else if (bottom > 0)
        {
          for (int i = m_height*width; i < height*width; ++i)
          {
            // v[i] = 0
            m_grid[i].reset();
          }
        }

        // Decrease height from top
        if (top < 0)
        {
          // v = v[abs(height_diff*width):]
          m_grid.erase(m_grid.begin(), m_grid.begin()+height_diff*width+1);
        }
        // Increase height to top
        else if (top > 0)
        {
          for (int i = width*height-1; i > -1; --i)
          {
            if (i < width*height_diff)
            {
              // v[i] = 0
              m_grid[i].reset();
            }
            else
            {
              // v[i] = v[i-height_diff*width]
              // m_grid[i] = std::move(m_grid[i-height_diff*width]);
              m_grid[i] = m_grid[i-height_diff*width];
            }
            
          }
        }

        m_width = width;
        m_height = height;
      }
  };

} // namespace core
} // namespace stella
