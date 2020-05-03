#pragma once

#include "renderer2.hpp"
#include <vector>
#include <glm/glm.hpp>

typedef unsigned int GLuint;
typedef float GLfloat;
typedef int GLsizei;

namespace stella
{
namespace graphics
{
  class Sprite;
  
  class ShapeRendererT : public RendererT
  {
  public:
    ShapeRendererT();
    ~ShapeRendererT();
    void begin();
    void submit (entt::registry& registry, entt::entity entity);
    void end();
    void draw();

  private:
    struct ShapeVertexData
    {
      glm::vec3 vertex;
      unsigned int color;
    };

    enum Index
    {
      VERTEX_INDEX,
      COLOR_INDEX
    };

    ShapeVertexData* m_vertex_buffer;

    static const unsigned S_MAX_SHAPES  = 10000;
    static const unsigned S_VERTEX_SIZE = sizeof (ShapeVertexData);
    // TODO: expand to draw shapes with more than 4 vertices
    static const unsigned S_SHAPE_SIZE   = 4 * S_VERTEX_SIZE;
    static const unsigned S_BUFFER_SIZE  = S_MAX_SHAPES * S_SHAPE_SIZE;
    static const unsigned S_INDICES_SIZE = 6 * S_MAX_SHAPES;

  protected:
    GLuint m_VAO, m_VBO, m_EBO;
    GLsizei m_index_count;
    bool textures_binded;
    std::vector<glm::mat4> m_transformation_stack;
    glm::mat4* m_transformation_back;

    void init();
  };
} // namespace graphics
} // namespace stella
