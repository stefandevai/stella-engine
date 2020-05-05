#pragma once

#include "renderable.hpp"
#include <glm/glm.hpp>
#include <tuple>
#include <vector>

namespace stella
{
namespace graphics
{
  class Shape : public Renderable
  {
  private:
    std::vector<glm::vec2> m_vertices;
    unsigned m_color = 0;

  public:
    Shape (const std::vector<glm::vec2>& vertices,
           const glm::vec3& position,
           const glm::vec4& color = glm::vec4{1.f, 1.f, 1.f, 1.f});

    // inline const glm::vec3 position() const { return m_position; }
    inline const std::vector<glm::vec2> vertices() const { return m_vertices; }
    inline const unsigned color() const { return m_color; }
    // inline const double width() const { return m_width; }
    // inline const double height() const { return m_height; }
    double calc_width();
    double calc_height();

    void set_vertex (const unsigned vertex, const float x, const float y);
    inline void set_color (const unsigned color) { m_color = color; }
  };

} // namespace graphics
} // namespace stella
