#include "renderable.h"

namespace stella { namespace graphics {
  Renderable::Renderable(GLint x, GLint y, GLint w, GLint h, Texture &texture, GLboolean visible)
    : Tex(texture)
  {
    this->Pos = glm::vec2((GLfloat)x, (GLfloat)y);
    this->Dimensions = glm::vec2((GLfloat)w, (GLfloat)h);
    this->Visible = visible;
  }
  
  Renderable::Renderable(GLint x, GLint y, Texture &texture)
    : Tex(texture)
  {
    this->Pos = glm::vec2((GLfloat)x, (GLfloat)y);
    this->Dimensions = glm::vec2(this->Tex.GetWidth(), this->Tex.GetWidth());
    this->Visible = GL_TRUE;
  }
  
  Renderable::Renderable(Texture &texture)
    : Tex(texture)
  {
    this->Pos = glm::vec2(0.0f, 0.0f);
    this->Dimensions = glm::vec2(this->Tex.GetWidth(), this->Tex.GetHeight());
    this->Visible = GL_TRUE;
  }

  Renderable::~Renderable()
  {
    
  }
} }

