#pragma once

#include <vector>
#include <GL/glew.h>
#include "../../glm/glm/glm.hpp"

#include "texture.h"
#include "spritesheet.h"

namespace stella { namespace graphics { 
  struct VertexData
  {
    glm::vec3 vertex;
    glm::vec2 uv;
    GLfloat tid;
    unsigned int color;
  };

  class Sprite {
    public:
      glm::vec2 Pos, Dimensions;

      Sprite(GLint x, GLint y, Texture &texture, const std::vector<GLuint> &frames);
      Sprite(GLint x, GLint y, Texture &texture, GLuint frame = 0);
      Sprite(GLint x, GLint y, GLint w, GLint h, Texture &texture, GLuint frame = 0);
      ~Sprite();

      // Getters
      inline const glm::vec2 GetPos() const { return Pos; }
      inline const glm::vec2 GetDimensions() const { return Dimensions; }
      inline const glm::vec4 GetColor() const { return Color; }
      inline const GLuint GetCurrentFrame() const { return Frame; }
      inline const GLuint GetTexID() const { return Tex.GetID(); }
      inline const std::string& GetTexName() const { return Tex.GetName(); }
      inline SpriteSheet* GetSpriteSheet() const { return Sprites; }
      inline Texture* GetTexture() const { return &Tex; }
      inline const GLboolean IsVisible() const { return Visible; }
      inline const glm::vec2 GetFrameCoords() const { return Sprites->GetUV(this->Frame); }

      // Setters
      void SetColor(int r, int g, int b) { Color.x = r/255.0f; Color.y = g/255.0f; Color.z = b/255.0f; }
      void SetFrame(GLuint frame);
      void SetVisible(GLboolean vis) { Visible = vis; }
      void ToggleVisible() { Visible = !Visible; }

    private:
      glm::vec4 Color;
      Texture &Tex;
      std::vector<GLuint> Frames;
      GLuint Frame, NumberOfFrames;
      GLboolean Visible;
      SpriteSheet* Sprites;
  };
} }

