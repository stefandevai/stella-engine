#pragma once

#include <glm/vec3.hpp>
#include <vector>

namespace stella
{
namespace graphics
{
  class ShaderProgram;

  struct Vertex
  {
    glm::vec3 position;
    //glm::vec3 normal;
    //glm::vec3 tex_coords;
  };

  //struct Texture2
  //{
    //unsigned int id;
    //std::string type;
  //};

  class Mesh
  {
  public:
    Mesh (const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
    ~Mesh ();

    void draw (ShaderProgram& shader);

  private:
    unsigned int m_vao;
    unsigned int m_vbo;
    unsigned int m_ebo;
    const std::vector<Vertex> m_vertices;
    const std::vector<uint32_t> m_indices;
    //std::vector<Texture2> textures;

  private:
    void m_setup();
  };
}
}

