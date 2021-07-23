#pragma once

#include <glm/glm.hpp>
#include <iostream>
#include <string>

typedef float GLfloat;
typedef unsigned int GLuint;
typedef int GLint;
typedef unsigned char GLboolean;

namespace stella
{
namespace graphics
{
  class Renderable
  {
  public:
    glm::vec3 Pos;
    glm::vec2 Dimensions, RealDimensions;

    Renderable (glm::vec3 position, glm::vec2 dimensions, GLboolean visible = 1);
    // Renderable (glm::vec3 position);
    Renderable (GLint x, GLint y, GLint w, GLint h, GLboolean visible = 1);
    // Renderable (GLint x, GLint y);
    virtual ~Renderable();

    // Getters
    inline glm::vec3 GetPos() const { return Pos; }
    inline glm::vec2 GetDimensions() const { return Dimensions; }
    inline GLfloat GetWidth() const { return Dimensions.x; }
    inline GLfloat GetHeight() const { return Dimensions.y; }
    inline GLfloat GetRotation() const { return Rotation; }
    inline glm::vec2 GetScale() const { return Scale; }
    inline const GLboolean& IsVisible() const { return Visible; }

    // Setters
    inline void SetVisible (GLboolean vis) { Visible = vis; }
    inline void ToggleVisible() { Visible = !Visible; }
    inline void SetDimensions (glm::vec2 dimensions) { Dimensions = dimensions; }
    inline void SetRotation (const GLfloat rotation) { Rotation = rotation; }
    inline void SetRelativeScale (glm::vec2 scale) { Scale = scale; }
    inline void SetScale (glm::vec2 scale) { Scale = scale; }

    inline void SetDirectScale (glm::vec2 scale) { Scale = scale / (this->Dimensions); }
    // void SetDirectScale(glm::vec2 scale) { Scale = scale/(this->Dimensions);
    // this->Dimensions *= this->Scale; }

  protected:
    GLboolean Visible;
    GLfloat Rotation = 0.0;
    glm::vec2 Scale  = glm::vec2 (1.f, 1.f);
  };
} // namespace graphics
} // namespace stella
