#include "stella/physics2d/water_surface.h"

#include <iostream>

namespace stella
{
namespace physics2d
{

    WaterSurface::WaterSurface(const double width)
      : m_width(width)
    {

    }

    WaterSurface::~WaterSurface()
    {

    }

    void WaterSurface::update(const double dt)
    {
      auto updated_U = std::vector<double>(m_number_of_columns);

      for (unsigned i = 0; i < m_number_of_columns; ++i)
      {
        double height_variation = 0.0;

        if (i == 0)
        {
          height_variation = (1.0 + m_U[i+1])/2.0 - 2.0*m_U[i];
        }
        else if (i + 1 >= m_number_of_columns)
        {
           height_variation = (m_U[i-1] + 1.0)/2.0 - 2.0*m_U[i];
        }
        else
        {
           height_variation = (m_U[i-1] + m_U[i+1])/2.0 - 2.0*m_U[i];
        }

        m_V[i] += height_variation*dt;
        m_V[i] *= 0.99;
        updated_U[i] = m_U[i] + m_V[i];
      }

      for (unsigned i = 0; i < m_number_of_columns; ++i)
      {
        m_U[i] = updated_U[i];
      }
    }

}
}
