#pragma once

#include "renderer.hpp"
#include <vector>
#include <glm/glm.hpp>

typedef unsigned int GLuint;
typedef int GLsizei;

namespace stella
{
namespace graphics
{
  class Shape;

  struct ShapeVertexData
  {
    glm::vec3 vertex;
    unsigned int color;
    glm::vec3 barycentric;
    glm::vec3 position;
    glm::vec2 dimensions;
  };

  class ShapeRenderer : public Renderer
  {
  private:
    const unsigned S_MAX_SHAPES  = 10000;
    const unsigned S_VERTEX_SIZE = sizeof (ShapeVertexData);
    // TODO: expand to draw shapes with more than 4 vertices
    const unsigned S_SHAPE_SIZE   = 4 * S_VERTEX_SIZE;
    const unsigned S_BUFFER_SIZE  = S_MAX_SHAPES * S_SHAPE_SIZE;
    const unsigned S_INDICES_SIZE = 6 * S_MAX_SHAPES;

  public:
    ShapeRenderer();
    ~ShapeRenderer();
    void Begin();
    void Submit (const std::shared_ptr<Renderable> renderable);
    void Submit (const std::shared_ptr<Shape> shape);
    void End();
    void Draw();

  private:
    enum Index
    {
      VERTEX_INDEX,
      COLOR_INDEX,
      BARYCENTRIC_INDEX,
      POSITION_INDEX,
      DIMENSIONS_INDEX
    };
    ShapeVertexData* m_vertex_buffer;
    GLuint VAO, VBO, EBO;
    GLsizei IndexCount;
    std::vector<GLuint> m_indices;

    void init();
  };
} // namespace graphics
} // namespace stella
