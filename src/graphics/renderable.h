#pragma once

#include <vector>
#include "texture.h"
#include <GL/glew.h>
#include "../../Dependencies/glm/glm/glm.hpp"

namespace stella { namespace graphics {
  class Renderable
  {
    public:
      glm::vec2 Pos, Dimensions;

      Renderable(GLint x, GLint y, GLint w, GLint h, Texture &texture, GLboolean visible = GL_TRUE);
      Renderable(GLint x, GLint y, Texture &texture);
      Renderable(Texture &texture);
      virtual ~Renderable();

      inline const glm::vec2 GetPos() const { return Pos; }
      inline const glm::vec2 GetDimensions() const { return Dimensions; }
      inline const GLuint GetTexID() const { return Tex.GetID(); }
      inline const std::string& GetTexName() const { return Tex.GetName(); }
      inline Texture* GetTexture() const { return &Tex; }
      inline const GLboolean IsVisible() const { return Visible; }

      void SetVisible(GLboolean vis) { Visible = vis; }
      void ToggleVisible() { Visible = !Visible; }

    protected:
      Texture &Tex;
      GLboolean Visible;
  };
} }

