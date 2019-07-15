#pragma once

#include <vector>
#include <cmath>

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
      const double m_height = 32.0;

      // Max width of each water column
      // It can be a smaller value if m_width is not a multiple of 8.
      // E.g.: If m_width is 32, the width of a column will be 8.0;
      //       Otherwise, if m_width is 36, we will divide 36/8.0 to 
      //       get 4.5 and then ceil the value to 5 to get 5 columns
      //       with the same width of 7.2
      const double m_max_column_width = 8.0;

      // Number of columns based on m_width provided and a const
      // m_max_column_width value
      const unsigned m_number_of_columns = static_cast<unsigned>(ceil(m_width/m_max_column_width));

      // Actual width of each column
      const double m_column_width = m_width/m_number_of_columns;

      // Height of water columns (values clamped to [0.5, 1.5])
      std::vector<double> m_U = std::vector<double>(m_number_of_columns, 1.0);

      // Vertical velocity of water columns
      std::vector<double> m_V = std::vector<double>(m_number_of_columns, 0.0);

    public:
      WaterSurface(const double width);
      ~WaterSurface();
      
      void update(const double dt);

      inline const double width() const { return m_width; }
      inline const double height() const { return m_height; }
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
      }

  };

}
}
