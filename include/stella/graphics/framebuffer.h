#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "stella/graphics/display.h"

typedef unsigned int GLuint;
typedef unsigned int GLenum;

namespace stella
{
namespace graphics
{
class Framebuffer
{
public:
  explicit Framebuffer (stella::graphics::Display& display);
  ~Framebuffer();

  void Bind();
  static void Unbind();
  void Draw();

  void ActivateTexture (GLenum tex_id);
  inline GLuint GetTexture() const { return FBOtex; }

private:
  GLuint FBO, FBOtex, VAO, VBO;
  stella::graphics::Display& Display;
  glm::vec2 CurrentTextureResolution;

  void init();
  void RefreshTextureResolution();
};
}
}
