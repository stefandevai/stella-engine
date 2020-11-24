#include "stella/graphics/mesh.hpp"
#include "stella/graphics/opengl.hpp"
#include "stella/graphics/shader_program.hpp"
#include <spdlog/spdlog.h>

namespace stella
{
namespace graphics
{

  Mesh::Mesh (const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
    : m_vertices (vertices), m_indices (indices)
  {
    m_setup();
  }

  Mesh::~Mesh ()
  {
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_ebo);
    glDeleteBuffers(1, &m_vbo);
  }

  void Mesh::draw (ShaderProgram& shader)
  {
    glBindVertexArray(m_vao);
    glDrawElements (GL_LINES, m_indices.size(), GL_UNSIGNED_INT, (void*) 0);
    glBindVertexArray(0);
  }

  void Mesh::m_setup()
  {
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ebo);

    glBindVertexArray(m_vao);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(graphics::Vertex) * m_vertices.size(), &m_vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * m_indices.size(), &m_indices[0], GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray (0);
  }
}
}


