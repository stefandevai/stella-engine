#pragma once

#include <vector>
#include <cmath>
#include <cassert>

namespace stella
{
namespace physics2d
{

  class WaterSurface
  {
    private:
      // Width of the water surface
      const double m_width;
      
      // Height of each water column
      const double m_height = 128.0;

      // Maximum variation allowed for each water column
      const double m_max_slope = 30.0;

      // Base wave speed
      const double m_wave_speed = 50.0;

      // Max width of each water column
      // It can be a smaller value if m_width is not a multiple of 16.
      // E.g.: If m_width is 32, the width of a column will be 16.0;
      //       Otherwise, if m_width is 36, we will divide 36/16.0 to 
      //       get 2.25 and then ceil the value to 3 to get 3 columns
      //       with the same width of 12.0
      const double m_max_column_width = 16.0;

      // Number of columns based on m_width provided and a const
      // m_max_column_width value
      const unsigned m_number_of_columns = static_cast<unsigned>(ceil(m_width/m_max_column_width));

      // Actual width of each column
      const double m_column_width = m_width/m_number_of_columns;

      // Height of water columns (values clamped to [0.5, 1.5])
      std::vector<double> m_U = std::vector<double>(m_number_of_columns, 1.0);

      // Vertical velocity of water columns
      std::vector<double> m_V = std::vector<double>(m_number_of_columns, 0.0);

      // Whether or not the surface is active and there's movement in the water
      bool m_active = false;

    public:
      WaterSurface(const double width);
      ~WaterSurface();
      
      void update(const double dt);
      void perturbate(const double x, const double amount);

      inline const double width() const { return m_width; }
      inline const double height() const { return m_height; }
      inline const bool is_active() const { return m_active; }
      inline const unsigned number_of_columns() const { return m_number_of_columns; }
      inline const double column_width() const { return m_column_width; }
      inline const double column_height(const unsigned x)
      {
        assert(x < m_number_of_columns);
        return m_U[x];
      }

      inline void set_column_height(const unsigned x, const double height)
      {
        assert(x < m_number_of_columns);
        m_U[x] = height;
        activate();
      }

    private:
      inline void activate()
      {
        if (!m_active) m_active = true;
      }
  };

}
}
