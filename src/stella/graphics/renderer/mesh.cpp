#include "stella/graphics/renderer/mesh.hpp"
#include "stella/graphics/opengl.hpp"
#include "stella/graphics/shader_program.hpp"
#include "stella/graphics/texture2.hpp"
#include <spdlog/spdlog.h>

namespace stella
{
namespace graphics
{

  Mesh::Mesh (const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const TextureVector& textures)
    : m_vertices (vertices), m_indices (indices), m_textures (textures)
  {
    m_setup();
  }

  Mesh::~Mesh ()
  {
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_ebo);
    glDeleteBuffers(1, &m_vbo);
  }

  void Mesh::render (ShaderProgram& shader)
  {
    unsigned int diffuse_count = 1;
    unsigned int specular_count = 1;
    unsigned int normal_count = 1;

    for (unsigned int i = 0; i < m_textures.size(); ++i)
    {
      glActiveTexture (GL_TEXTURE0 + i);

      std::string number;
      std::string name;

      switch (m_textures[i]->get_type())
      {
        case TextureType::DIFFUSE:
          {
            name = "texture_diffuse";
            number = std::to_string(diffuse_count++);
          }
          break;

        case TextureType::SPECULAR:
          {
            name = "texture_specular";
            number = std::to_string(specular_count++);
          }
          break;

        case TextureType::NORMAL:
          {
            name = "texture_normal";
            number = std::to_string(normal_count++);
          }
          break;

        default:
          break;
      }

      shader.set_float ((name + number), static_cast<float>(i));
      glBindTexture (GL_TEXTURE_2D, m_textures[i]->get_id());
    }
    glActiveTexture (GL_TEXTURE0);

    glBindVertexArray(m_vao);
    glDrawElements (GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, (void*) 0);
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

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) 0);
    glEnableVertexAttribArray(0);

    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof (Vertex, normal));
    glEnableVertexAttribArray(1);

    // texcoords attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof (Vertex, texcoords));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray (0);
  }
}
}


