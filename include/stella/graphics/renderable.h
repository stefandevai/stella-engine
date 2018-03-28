#pragma once

#include "texture.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

namespace stella {
namespace graphics {
class Renderable {
public:
  glm::vec2 Pos, Dimensions;

  Renderable(GLint x, GLint y, GLint w, GLint h, Texture &texture,
             GLboolean visible = GL_TRUE);
  Renderable(GLint x, GLint y, Texture &texture);
  Renderable(Texture &texture);
  virtual ~Renderable();

  // Getters
  inline const glm::vec2 GetPos() const { return Pos; }
  inline const glm::vec2 GetDimensions() const { return Dimensions; }
  inline const GLfloat GetWidth() const { return Dimensions.x; }
  inline const GLfloat GetHeight() const { return Dimensions.y; }
  inline const GLfloat GetRotation() const { return Rotation; }
  inline const glm::vec2 GetScale() const { return Scale; }
  inline const GLuint GetTexID() const { return Tex.GetID(); }
  inline const std::string &GetTexName() const { return Tex.GetName(); }
  inline Texture *GetTexture() const { return &Tex; }
  inline const GLboolean& IsVisible() const { return Visible; }

  // Setters
  void SetVisible(GLboolean vis) { Visible = vis; }
  void ToggleVisible() { Visible = !Visible; }
  void SetRotation(float rotation) { Rotation = rotation; }
  void SetScale(glm::vec2 scale) { Scale = scale; }

protected:
  Texture &Tex;
  GLboolean Visible;
  float Rotation = 0.f;
  glm::vec2 Scale = glm::vec2(1.f,1.f);
};
} // namespace graphics
} // namespace stella
