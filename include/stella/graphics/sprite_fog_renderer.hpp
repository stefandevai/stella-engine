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
  class Texture;
  class SpriteFogRenderer : public Renderer
  {
    public:
        SpriteFogRenderer();
        ~SpriteFogRenderer();
        void Begin();
        void Submit (const std::shared_ptr<Renderable> renderable);
        void Submit (const std::shared_ptr<Sprite> sprite);
        void End();
        void Draw();
    private:
      struct VertexData
      {
        glm::vec3 vertex;
        glm::vec2 uv;
        GLfloat tid;
        GLfloat yorigin;
        GLfloat height;
      };

      enum Index
      {
        VERTEX_INDEX,
        UV_INDEX,
        TID_INDEX,
        YORIGIN_INDEX,
        HEIGHT_INDEX
      };

      VertexData* VertexBuffer;

      static const unsigned MAX_SPRITES =  10000;
      static const unsigned VERTEX_SIZE = sizeof (VertexData);
      static const unsigned SPRITE_SIZE = 4 * VERTEX_SIZE;
      static const unsigned BUFFER_SIZE = MAX_SPRITES* SPRITE_SIZE;
      static const unsigned INDICES_SIZE = 6 * MAX_SPRITES;

      GLuint VAO, VBO, EBO;
      GLsizei IndexCount;
      bool TexturesBinded;
      std::vector<glm::mat4> TransformationStack;
      glm::mat4* TransformationBack;

      void init();
      // FogVertexData* VertexBuffer;
  };
}
}