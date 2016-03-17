#pragma once

#include <GL/glew.h>

namespace stella { namespace graphics {
  class Texture
  {
    public:
      Texture(const char* texPath);
      ~Texture();
      void Bind();

      inline const GLint GetWidth() const { return (GLint)Width; }
      inline const GLint GetHeight() const { return (GLint)Height; }

    private:
      GLuint ID, Width, Height;
      void load(const char* texPath);
  };
} }

