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
      for (unsigned i = 0; i < m_number_of_columns; ++i)
      {
        if (i == 0)
        {
          m_V[i] = (1.0 + m_U[i+1])/2.0 - m_U[i];
        }
        else if (i + 1 >= m_number_of_columns)
        {
          m_V[i] = (m_U[i-1] + 1.0)/2.0 - m_U[i];
        }
        else
        {
          m_V[i] = (m_U[i-1] + m_U[i+1])/2.0 - m_U[i];
        }

        m_V[i] *= 0.99;
        m_U[i] += m_V[i];
      }
    }

}
}
