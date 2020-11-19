#include "stella/physics2d/water_surface.hpp"

#include <math.h>

namespace stella
{
namespace physics2d
{
  WaterSurface::WaterSurface (const double width) : m_width (width) {}

  WaterSurface::~WaterSurface() {}

  void WaterSurface::update (const double dt)
  {
    if (m_active)
    {
      double corrected_wave_speed = m_wave_speed;
      if (corrected_wave_speed >= m_height / dt)
        corrected_wave_speed = m_height / dt - 1.0;
      auto updated_U = std::vector<double> (m_number_of_columns);

      // We will check if there's variation in any column's height
      // If not, we will deactivate the surface to avoid unecessary updates
      double max_offset = 0.0;
      double min_offset = 0.0;

      for (unsigned i = 0; i < m_number_of_columns; ++i)
      {
        // Offset for a column height
        double offset = 0.0;

        // Deals with out of bounds values in vector m_U
        // E.g.: m_U[-1] or m_U[m_number_of_columns + 1]
        if (i == 0)
        {
          // offset = pow(corrected_wave_speed, 2)*(1.0 + m_U[i+1]
          // - 2.0*m_U[i])/pow(m_height, 2);
          offset = (1.0 + m_U[i + 1]) / 2.0 - 2.0 * m_U[i];
        }
        else if (i + 1 >= m_number_of_columns)
        {
          // offset = pow(corrected_wave_speed, 2)*(m_U[i-1] + 1.0
          // - 2.0*m_U[i])/pow(m_height, 2);
          offset = (m_U[i - 1] + 1.0) / 2.0 - 2.0 * m_U[i];
        }
        else
        {
          // offset = pow(corrected_wave_speed, 2)*(m_U[i-1] + m_U[i+1]
          // - 2.0*m_U[i])/pow(m_height, 2);
          offset = (m_U[i - 1] + m_U[i + 1]) / 2.0 - 2.0 * m_U[i];
        }

        if (offset < min_offset)
          min_offset = offset;
        else if (offset > max_offset)
          max_offset = offset;

        offset *= dt;

        // Clampling too high or too low values
        if (offset > m_max_slope)
          offset = m_max_slope;
        else if (offset < -m_max_slope)
          offset = -m_max_slope;

        m_V[i] += offset;
        m_V[i] *= 0.99;
        updated_U[i] = m_U[i] + m_V[i];
      }

      // If height variation in columns is less than 0.9
      // we deactivate the water surface
      if (max_offset - min_offset < 0.1)
      {
        m_active = false;
      }
      else
      {
        for (unsigned i = 0; i < m_number_of_columns; ++i)
        {
          m_U[i] = updated_U[i];
        }
      }
    }
  }

  void WaterSurface::perturbate (const double x, const double amount)
  {
    assert (x <= m_width);
    const unsigned int column = x / m_column_width;

    this->set_column_height (column, amount);
    if (column + 1 < m_number_of_columns)
      this->set_column_height (column + 1, amount * 0.8);
    if (column - 1 > 0)
      this->set_column_height (column - 1, amount * 0.8);
    if (column + 2 < m_number_of_columns)
      this->set_column_height (column + 2, amount * 0.4);
    if (column - 2 > 0)
      this->set_column_height (column - 2, amount * 0.4);
  }

} // namespace physics2d
} // namespace stella
