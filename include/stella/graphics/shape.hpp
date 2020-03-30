#pragma once

#include <glm/glm.hpp>
#include <tuple>
#include <vector>

typedef float GLfloat;

namespace stella
{
namespace graphics
{
  class Shape
  {
  private:
    std::vector<glm::vec2> m_vertices;
    glm::vec3 m_position;
    unsigned m_color = 0;
    double m_width, m_height;

  public:
    inline Shape (const std::vector<glm::vec2>& vertices,
                  const glm::vec3& position,
                  const glm::vec4& color = glm::vec4{1.f, 1.f, 1.f, 1.f})
      : m_vertices (vertices), m_position (position)
    {
      m_color = static_cast<unsigned> (color.a * 255) << 24 | static_cast<unsigned> (color.b * 255) << 16 |
                static_cast<unsigned> (color.g * 255) << 8 | static_cast<unsigned> (color.r * 255);
      double maxx, maxy, minx, miny;
      maxx = vertices[0].x;
      maxy = vertices[0].y;
      minx = vertices[0].x;
      miny = vertices[0].y;

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
        if (vertex.y > maxy)
        {
          maxy = vertex.y;
        }
        else if (vertex.y < miny)
        {
          miny = vertex.y;
        }
      }
      m_width  = maxx - minx;
      m_height = maxy - miny;
    }
    ~Shape() {}

    inline const glm::vec3 position() const { return m_position; }
    inline const std::vector<glm::vec2> vertices() const { return m_vertices; }
    inline const unsigned color() const { return m_color; }
    inline const double width() const { return m_width; }
    inline const double height() const { return m_height; }

    inline void set_vertex (const unsigned vertex, const float x, const float y)
    {
      assert (vertex < m_vertices.size());
      m_vertices[vertex].x = x;
      m_vertices[vertex].y = y;
    }
  };

} // namespace graphics
} // namespace stella
