#pragma once

#include "texture.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <iostream>

namespace stella {
namespace graphics {
class Renderable {
public:
  glm::vec2 Pos, Dimensions, RealDimensions;

  Renderable(GLint x, GLint y, GLint w, GLint h, Texture &texture,
             GLboolean visible = GL_TRUE);
  Renderable(GLint x, GLint y, Texture &texture);
  Renderable(Texture &texture);
  virtual ~Renderable();

  // Getters
  inline glm::vec2 GetPos() const { return Pos; }
  inline glm::vec2 GetDimensions() const { return Dimensions; }
  inline GLfloat GetWidth() const { return Dimensions.x; }
  inline GLfloat GetHeight() const { return Dimensions.y; }
  inline GLfloat GetRotation() const { return Rotation; }
  inline glm::vec2 GetScale() const { return Scale; }
  inline GLuint GetTexID() const { return Tex.GetID(); }
  inline const std::string &GetTexName() const { return Tex.GetName(); }
  inline Texture *GetTexture() const { return &Tex; }
  inline const GLboolean& IsVisible() const { return Visible; }

  // Setters
  void SetVisible(GLboolean vis) { Visible = vis; }
  void ToggleVisible() { Visible = !Visible; }
  void SetDimensions(glm::vec2 dimensions) { Dimensions = dimensions; }
  void SetRotation(float rotation) { Rotation = rotation; }
  void SetRelativeScale(glm::vec2 scale) { Scale = scale; }
  void SetScale(glm::vec2 scale) { Scale = scale; }
  
  void SetDirectScale(glm::vec2 scale) { Scale = scale/(this->Dimensions);  }
  //void SetDirectScale(glm::vec2 scale) { Scale = scale/(this->Dimensions); this->Dimensions *= this->Scale; }

protected:
  Texture &Tex;
  GLboolean Visible;
  float Rotation = 0.f;
  glm::vec2 Scale = glm::vec2(1.f,1.f);
};
} // namespace graphics
} // namespace stella
