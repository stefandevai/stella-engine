#pragma once

#include "renderer.hpp"
#include <vector>
#include <glm/glm.hpp>

#define MAX_SPRITES  10000
#define VERTEX_SIZE  sizeof (VertexData)
#define SPRITE_SIZE  4 * VERTEX_SIZE
#define BUFFER_SIZE  MAX_SPRITES* SPRITE_SIZE
#define INDICES_SIZE 6 * MAX_SPRITES

typedef unsigned int GLuint;
typedef int GLsizei;

namespace stella
{
namespace graphics
{
  class Sprite;
  class Texture;
  struct VertexData;

  class SpriteRenderer : public Renderer
  {
  public:
    SpriteRenderer();
    ~SpriteRenderer();
    void Begin();
    void Submit (const Sprite& sprite);
    void End();
    void Draw();

    static void BindAsRenderTarget (int width = 720, int height = 405);

    void PushTransformation (glm::mat4& mat, bool override = false);
    void PopTransformation();

    static std::vector<Texture*> Textures;

  private:
    enum Index
    {
      VERTEX_INDEX,
      UV_INDEX,
      TID_INDEX,
      COLOR_INDEX
    };
    VertexData* VertexBuffer;
    GLuint VAO, VBO, EBO;
    std::vector<glm::mat4> TransformationStack;
    glm::mat4* TransformationBack;
    GLsizei IndexCount;
    bool TexturesBinded;

    void init();
  };
} // namespace graphics
} // namespace stella
