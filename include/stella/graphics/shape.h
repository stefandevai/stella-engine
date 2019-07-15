#pragma once

#include <tuple>
#include <glm/glm.hpp>

typedef float GLfloat;

namespace stella {
namespace graphics {

  class Shape {
    private:
      std::vector<glm::vec2> m_vertices;
      glm::vec3 m_position;
      unsigned m_color = 0;

    public:
      inline Shape(const std::vector<glm::vec2> &vertices, const glm::vec3 &position, const glm::vec4 &color = glm::vec4{1.f, 1.f, 1.f, 1.f})
        : m_vertices(vertices), m_position(position)
      {
        m_color = static_cast<unsigned>(color.a * 255) << 24 | static_cast<unsigned>(color.b * 255) << 16 | static_cast<unsigned>(color.g * 255) << 8 | static_cast<unsigned>(color.r * 255);
      }
      ~Shape() {}

      inline const glm::vec3 position() const { return m_position; }
      inline const std::vector<glm::vec2> vertices() const { return m_vertices; }
      inline const unsigned color() const { return m_color; }

      inline void set_vertex(const unsigned vertex, const float x, const float y)
      {
        assert(vertex < m_vertices.size());
        m_vertices[vertex].x = x;
        m_vertices[vertex].y = y;
      }
  };

} // namespace graphics
} // namespace stella
