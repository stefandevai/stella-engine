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

      // Max width of each water column
      // It can be a smaller value if m_width is not a multiple of 8.
      // E.g.: If m_width is 32, the width of a column will be 8.0;
      //       Otherwise, if m_width is 36, we will divide 36/8.0 to 
      //       get 4.5 and then ceil the value to 5 to get 5 columns
      //       with the same width of 7.2
      const double m_max_column_width = 8.0;

      // Number of columns based on m_width provided and a const
      // m_max_column_width value
      const unsigned number_of_columns = static_cast<unsigned>(ceil(m_width/m_max_column_width));

      // Actual width of each column
      const double m_column_width = m_width/number_of_columns;

      // Height of water columns (values clamped to [0.5, 1.5])
      std::vector<double> m_U = std::vector<double>(number_of_columns, 1.0);

      // Vertical velocity of water columns
      std::vector<double> m_V = std::vector<double>(number_of_columns, 0.0);

    public:
      WaterSurface(const double width);
      ~WaterSurface();

      inline const double width() const { return m_width; }
      inline const double column_width() const { return m_column_width; }
      inline const std::vector<double> u_vector() const { return m_U; }
      inline const std::vector<double> v_vector() const { return m_V; }

  };

}
}
