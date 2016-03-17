#pragma once

#include <GL/glew.h>

#include "sprite.h"

#define MAX_SPRITES 10000
#define VERTEX_SIZE sizeof(VertexData)
#define SPRITE_SIZE 4 * VERTEX_SIZE
#define BUFFER_SIZE MAX_SPRITES * SPRITE_SIZE
#define INDICES_SIZE 6 * MAX_SPRITES


namespace stella { namespace graphics { 
  enum Index {
    VERTEX_INDEX,
    COLOR_INDEX
  };

  class Renderer
  {
    public:
      Renderer();
      ~Renderer();
      void Begin();
      void Submit(const Sprite &sprite);
      void End();
      void Draw();

    private:
      VertexData *VertexBuffer;
      GLuint VAO, VBO, EBO;
      GLsizei IndexCount;

      void init();
  };
} }

