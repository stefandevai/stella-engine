#include "stella/graphics/shape_renderer.hpp"
#include "stella/graphics/texture.hpp"
#include "stella/graphics/opengl.hpp" // IWYU pragma: export
#include "stella/components/shape.hpp"
#include "stella/components/position.hpp"
#include "stella/components/color.hpp"
#include "stella/components/transform.hpp"
#include "stella/components/dimension.hpp"
#include "stella/components/vertical.hpp"

#include <algorithm>
#include <cstddef>

#include <entt/entity/registry.hpp>
#include <glm/gtc/matrix_transform.hpp> // IWYU pragma: export

namespace stella
{
namespace graphics
{
  ShapeRendererT::ShapeRendererT() : RendererT()
  {
    m_transformation_stack.push_back (glm::mat4());
    m_transformation_back = &m_transformation_stack.back();
    this->init();
  }

  ShapeRendererT::~ShapeRendererT()
  {
    glDeleteBuffers (1, &this->m_VBO);
    glDeleteBuffers (1, &this->m_EBO);
    glDeleteVertexArrays (1, &this->m_VAO);
  }

  void ShapeRendererT::init()
  {
    m_index_count = 0;
    glGenVertexArrays (1, &m_VAO);
    glGenBuffers (1, &m_VBO);
    glGenBuffers (1, &m_EBO);
    glBindVertexArray (m_VAO);
    glBindBuffer (GL_ARRAY_BUFFER, m_VBO);
    glBufferData (GL_ARRAY_BUFFER, S_BUFFER_SIZE, NULL, GL_STATIC_DRAW);
    glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, m_EBO);

    GLint offset = 0;
    GLuint indices[S_INDICES_SIZE];
    for (GLuint i = 0; i < S_INDICES_SIZE; i += 6)
    {
      indices[i]     = offset;
      indices[i + 1] = offset + 1;
      indices[i + 2] = offset + 3;

      indices[i + 3] = offset + 1;
      indices[i + 4] = offset + 2;
      indices[i + 5] = offset + 3;

      offset += 4;
    }

    glBufferData (GL_ELEMENT_ARRAY_BUFFER, sizeof (indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer (VERTEX_INDEX, 3, GL_FLOAT, GL_FALSE, S_VERTEX_SIZE, (void*) 0);
    glEnableVertexAttribArray (VERTEX_INDEX);

    glVertexAttribPointer (COLOR_INDEX, 4, GL_UNSIGNED_BYTE, GL_TRUE, S_VERTEX_SIZE, (GLvoid*) offsetof (ShapeVertexData, color));
    glEnableVertexAttribArray (COLOR_INDEX);

    glBindBuffer (GL_ARRAY_BUFFER, 0);
    glBindVertexArray (0);
  }

  void ShapeRendererT::begin()
  {
    glBindBuffer (GL_ARRAY_BUFFER, m_VBO);
    m_vertex_buffer = static_cast<ShapeVertexData*> (glMapBuffer (GL_ARRAY_BUFFER, GL_WRITE_ONLY));
  }

  void ShapeRendererT::submit (entt::registry& registry, entt::entity entity)
  {
    auto& shape                            = registry.get<component::Shape> (entity);
    auto& pos                              = registry.get<component::Position> (entity);
    auto& color                            = registry.get<component::Color> (entity);
    const glm::vec3 position               = glm::vec3 (pos.x, pos.y, pos.z);
    const std::vector<glm::vec3>& vertices = shape.vertices;
    const unsigned int c                   = color.int_color;
    const auto dim                         = shape.calc_dimensions();

    auto particular_transform = *m_transformation_back;
    if (registry.any_of<component::Vertical> (entity))
    {
      // Translation before scale and rotation
      // Also we multiply the height (dim.y) by sin(45deg) in order to compensate
      // the decrease in z position after rotation
      particular_transform = glm::translate (particular_transform, glm::vec3 (position.x, position.y, position.z + dim.y * 0.70710678118f));

      // Scale in the y axis by 1 / (cos 45deg) in order to compensate for the scale reduction when rotating
      particular_transform = glm::scale (particular_transform, glm::vec3 (1.f, 1.41421356237f, 1.f));

      // Rotate -45deg in the x axis from the top in order to have depth information for lighting and other effects
      particular_transform = glm::rotate (particular_transform, glm::radians (-45.f), glm::vec3 (1.f, 0.f, 0.f));
    }

    else
    {
      auto trans = component::Transform();
      if (registry.any_of<component::Transform> (entity))
      {
        trans = registry.get<component::Transform> (entity);
      }
      // // Translating half dimension to set the point of rotation to the center of the sprite
      particular_transform = glm::translate (particular_transform, position + glm::vec3 (dim.x, dim.y, 0.f) / 2.f);
      particular_transform = glm::scale (particular_transform, trans.scale);
      particular_transform = glm::rotate (particular_transform, glm::radians (trans.rotation.x), glm::vec3 (1.f, 0.f, 0.f));
      particular_transform = glm::rotate (particular_transform, glm::radians (trans.rotation.y), glm::vec3 (0.f, 1.f, 0.f));
      particular_transform = glm::rotate (particular_transform, glm::radians (trans.rotation.z), glm::vec3 (0.f, 0.f, 1.f));
      // Removing the added half dimension
      particular_transform = glm::translate (particular_transform, glm::vec3 (-dim.x, -dim.y, 0.f) / 2.f);
    }

    for (auto& vertex : vertices)
    {
      auto transformation_result = particular_transform * glm::vec4 (vertex.x, vertex.y, vertex.z, 1.f);
      m_vertex_buffer->vertex    = glm::vec3 (transformation_result.x, transformation_result.y, transformation_result.z);
      m_vertex_buffer->color     = c;
      ++m_vertex_buffer;
    }

    // // TODO: Update when allowing shapes with more than 4 vertices
    this->m_index_count += 6;
  }

  void ShapeRendererT::end()
  {
    glUnmapBuffer (GL_ARRAY_BUFFER);
    glBindBuffer (GL_ARRAY_BUFFER, 0);
  }

  void ShapeRendererT::draw()
  {
    glBindVertexArray (m_VAO);
    glDrawElements (GL_TRIANGLES, m_index_count, GL_UNSIGNED_INT, 0);
    glBindVertexArray (0);
  }

} // namespace graphics
} // namespace stella
