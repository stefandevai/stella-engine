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
  

  class SpriteRenderer : public Renderer
  {
  public:
    SpriteRenderer();
    ~SpriteRenderer();
    virtual void Begin();
    void Submit (const std::shared_ptr<Renderable> renderable);
    virtual void Submit (const std::shared_ptr<Sprite> sprite);
    void End();
    void Draw();

    static void BindAsRenderTarget (int width = 720, int height = 405);

    void PushTransformation (glm::mat4& mat, bool override = false);
    void PopTransformation();

    //static std::vector<Texture*> Textures;

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

    VertexData* VertexBuffer;

    static const unsigned MAX_SPRITES =  10000;
    static const unsigned VERTEX_SIZE = sizeof (VertexData);
    static const unsigned SPRITE_SIZE = 4 * VERTEX_SIZE;
    static const unsigned BUFFER_SIZE = MAX_SPRITES* SPRITE_SIZE;
    static const unsigned INDICES_SIZE = 6 * MAX_SPRITES;

  protected:
    GLuint VAO, VBO, EBO;
    std::vector<glm::mat4> TransformationStack;
    glm::mat4* TransformationBack;
    GLsizei IndexCount;
    bool TexturesBinded;

    virtual void init();
  };
} // namespace graphics
} // namespace stella
