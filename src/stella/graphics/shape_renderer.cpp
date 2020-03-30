#include "stella/graphics/shape_renderer.hpp"
#include "stella/graphics/shape.hpp"

#include "stella/graphics/opengl.hpp"

#include <algorithm>
#include <cstddef>

#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

namespace stella
{
namespace graphics
{
  ShapeRenderer::ShapeRenderer() { this->init(); }

  ShapeRenderer::~ShapeRenderer()
  {
    glDeleteBuffers (1, &this->VBO);
    glDeleteBuffers (1, &this->EBO);
    glDeleteVertexArrays (1, &this->VAO);
    m_indices.reserve (S_INDICES_SIZE);
  }

  void ShapeRenderer::init()
  {
    this->IndexCount = 0;
    glGenVertexArrays (1, &this->VAO);
    glGenBuffers (1, &this->VBO);
    glGenBuffers (1, &this->EBO);

    glBindVertexArray (this->VAO);
    glBindBuffer (GL_ARRAY_BUFFER, this->VBO);
    glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, this->EBO);

    glBufferData (GL_ARRAY_BUFFER, S_BUFFER_SIZE, NULL, GL_DYNAMIC_DRAW);
    glVertexAttribPointer (VERTEX_INDEX, 3, GL_FLOAT, GL_FALSE, S_VERTEX_SIZE, (GLvoid*) 0);
    glEnableVertexAttribArray (VERTEX_INDEX);

    glVertexAttribPointer (
        COLOR_INDEX, 4, GL_UNSIGNED_BYTE, GL_TRUE, S_VERTEX_SIZE, (GLvoid*) offsetof (ShapeVertexData, color));
    glEnableVertexAttribArray (COLOR_INDEX);

    glVertexAttribPointer (
        BARYCENTRIC_INDEX, 3, GL_FLOAT, GL_FALSE, S_VERTEX_SIZE, (GLvoid*) offsetof (ShapeVertexData, barycentric));
    glEnableVertexAttribArray (BARYCENTRIC_INDEX);

    glVertexAttribPointer (
        POSITION_INDEX, 3, GL_FLOAT, GL_FALSE, S_VERTEX_SIZE, (GLvoid*) offsetof (ShapeVertexData, position));
    glEnableVertexAttribArray (POSITION_INDEX);

    glVertexAttribPointer (
        DIMENSIONS_INDEX, 2, GL_FLOAT, GL_FALSE, S_VERTEX_SIZE, (GLvoid*) offsetof (ShapeVertexData, dimensions));
    glEnableVertexAttribArray (DIMENSIONS_INDEX);

    glBindBuffer (GL_ARRAY_BUFFER, 0);

    GLint offset = 0;
    m_indices.clear();
    for (GLuint i = 0; i < S_INDICES_SIZE; i += 6)
    {
      m_indices.push_back (offset);
      m_indices.push_back (offset + 1);
      m_indices.push_back (offset + 2);

      m_indices.push_back (offset);
      m_indices.push_back (offset + 2);
      m_indices.push_back (offset + 3);

      offset += 4;
    }
    glBufferData (GL_ELEMENT_ARRAY_BUFFER, sizeof (m_indices), &m_indices, GL_STATIC_DRAW);

    glBindVertexArray (0);
  }

  void ShapeRenderer::Begin()
  {
    glBindBuffer (GL_ARRAY_BUFFER, this->VBO);
    m_vertex_buffer = static_cast<ShapeVertexData*> (glMapBuffer (GL_ARRAY_BUFFER, GL_WRITE_ONLY));
  }

  void ShapeRenderer::Submit (const Shape& shape)
  {
    const glm::vec3& position              = shape.position();
    const std::vector<glm::vec2>& vertices = shape.vertices();
    // const glm::vec2 &dimensions = sprite.GetDimensions();
    // const float rotation = sprite.GetRotation();
    // const glm::vec2 &scale = sprite.GetScale();
    const unsigned int c = shape.color();

    // auto particular_transform = glm::mat4();
    // particular_transform = glm::translate(particular_transform,
    // glm::vec3(position + glm::vec3(dimensions, 0.f)/2.f)); // Translating half
    // dimension to set the point of rotation to the center of the sprite
    // particular_transform = glm::scale(particular_transform,
    // glm::vec3(scale, 1.f)); particular_transform =
    // glm::rotate(particular_transform, glm::radians(rotation), glm::vec3(0.f,
    // 0.f, 1.f)); particular_transform = glm::translate(particular_transform,
    // glm::vec3(-dimensions/2.f, 0.f)); // Removing the added half dimension

    unsigned counter = 1;
    for (auto& vertex : vertices)
    {
      m_vertex_buffer->vertex     = glm::vec3 (position.x + vertex.x, position.y + vertex.y, position.z);
      m_vertex_buffer->color      = c;
      m_vertex_buffer->position   = position;
      m_vertex_buffer->dimensions = glm::vec2 (shape.width(), shape.height());

      if (counter % 4 == 0)
      {
        m_vertex_buffer->barycentric = glm::vec3 (0.f, 1.f, 0.f);
      }
      else if (counter % 3 == 0)
      {
        m_vertex_buffer->barycentric = glm::vec3 (0.f, 0.f, 1.f);
      }
      else if (counter % 2 == 0)
      {
        m_vertex_buffer->barycentric = glm::vec3 (0.f, 1.f, 0.f);
      }
      else
      {
        m_vertex_buffer->barycentric = glm::vec3 (1.f, 0.f, 0.f);
      }

      ++m_vertex_buffer;
      ++counter;
    }

    // TODO: Update when allowing shapes with more than 4 vertices
    this->IndexCount += 6;
  }

  void ShapeRenderer::End()
  {
    glUnmapBuffer (GL_ARRAY_BUFFER);
    glBindBuffer (GL_ARRAY_BUFFER, 0);
  }

  void ShapeRenderer::Draw()
  {
    glBindVertexArray (this->VAO);
    glDrawElements (GL_TRIANGLES, this->IndexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray (0);
    this->IndexCount = 0;
  }

} // namespace graphics
} // namespace stella
