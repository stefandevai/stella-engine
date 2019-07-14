#pragma once

#include <vector>

#include <glm/glm.hpp>

#define MAX_SPRITES 10000
#define VERTEX_SIZE sizeof(VertexData)
#define SPRITE_SIZE 4 * VERTEX_SIZE
#define BUFFER_SIZE MAX_SPRITES * SPRITE_SIZE
#define INDICES_SIZE 6 * MAX_SPRITES

typedef unsigned int GLuint;
typedef int GLsizei;

namespace stella {
namespace graphics {
class Sprite;
class Texture;
struct VertexData;

class ShapeRenderer {
public:
  ShapeRenderer();
  ~ShapeRenderer();
  void Begin();
  void Submit(const Sprite &sprite);
  static void End();
  void Draw();

private:
  enum Index { VERTEX_INDEX, UV_INDEX, TID_INDEX, COLOR_INDEX };
  VertexData *VertexBuffer;
  GLuint VAO, VBO, EBO;
  GLsizei IndexCount;
  std::vector<Texture *> Textures;
  bool TexturesBinded;

  void init();
};
} // namespace graphics
} // namespace stella

