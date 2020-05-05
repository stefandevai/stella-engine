#pragma once

#include "renderer.hpp"
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

  class SpriteRendererT : public RendererT
  {
  public:
    SpriteRendererT();
    ~SpriteRendererT();
    void begin();
    void submit (entt::registry& registry, entt::entity entity);
    void end();
    void draw();

  private:
    struct VertexData
    {
      glm::vec3 vertex;
      glm::vec2 uv;
      GLfloat tid;
      unsigned int color;
    };

    enum Index
    {
      VERTEX_INDEX,
      UV_INDEX,
      TID_INDEX,
      COLOR_INDEX
    };

    VertexData* m_vertex_buffer;

    static const unsigned MAX_SPRITES  = 10000;
    static const unsigned VERTEX_SIZE  = sizeof (VertexData);
    static const unsigned SPRITE_SIZE  = 4 * VERTEX_SIZE;
    static const unsigned BUFFER_SIZE  = MAX_SPRITES * SPRITE_SIZE;
    static const unsigned INDICES_SIZE = 6 * MAX_SPRITES;

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
