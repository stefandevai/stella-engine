#include "stella/graphics/shape.hpp"

namespace stella
{
namespace graphics
{
  Shape::Shape (const std::vector<glm::vec2>& vertices, const glm::vec3& position, const glm::vec4& color) : Renderable (position, glm::vec2()), m_vertices (vertices)
  {
    Dimensions.x = calc_width();
    Dimensions.y = calc_height();
    m_color      = static_cast<unsigned> (color.a * 255) << 24 | static_cast<unsigned> (color.b * 255) << 16 | static_cast<unsigned> (color.g * 255) << 8 |
              static_cast<unsigned> (color.r * 255);
    //   double maxx, maxy, minx, miny;
    //   maxx = vertices[0].x;
    //   maxy = vertices[0].y;
    //   minx = vertices[0].x;
    //   miny = vertices[0].y;

    //   for (const auto& vertex : m_vertices)
    //   {
    //     if (vertex.x > maxx)
    //     {
    //       maxx = vertex.x;
    //     }
    //     else if (vertex.x < minx)
    //     {
    //       minx = vertex.x;
    //     }
    //     if (vertex.y > maxy)
    //     {
    //       maxy = vertex.y;
    //     }
    //     else if (vertex.y < miny)
    //     {
    //       miny = vertex.y;
    //     }
    //   }
    //   m_width  = maxx - minx;
    //   m_height = maxy - miny;
  }
  double Shape::calc_width()
  {
    double maxx, minx;
    maxx = minx = m_vertices[0].x;
    for (const auto& vertex : m_vertices)
    {
      if (vertex.x > maxx)
      {
        maxx = vertex.x;
      }
      else if (vertex.x < minx)
      {
        minx = vertex.x;
      }
    }
    return maxx - minx;
  }
  double Shape::calc_height()
  {
    double maxy, miny;
    maxy = miny = m_vertices[0].y;
    for (const auto& vertex : m_vertices)
    {
      if (vertex.y > maxy)
      {
        maxy = vertex.y;
      }
      else if (vertex.y < miny)
      {
        miny = vertex.y;
      }
    }
    return maxy - miny;
  }
  void Shape::set_vertex (const unsigned vertex, const float x, const float y)
  {
    assert (vertex < m_vertices.size());
    m_vertices[vertex].x = x;
    m_vertices[vertex].y = y;
  }
} // namespace graphics
} // namespace stella
