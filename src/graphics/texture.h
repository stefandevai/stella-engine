#pragma once

#include <GL/glew.h>
#include <string>

namespace stella { namespace graphics {
  class Texture
  {
    public:
      Texture(const std::string name, const char* texPath);
      ~Texture();
      void Bind();
      void Unbind();

      inline const GLuint GetWidth() const { return (GLint)Width; }
      inline const GLuint GetHeight() const { return (GLint)Height; }
      inline const GLuint GetID() const { return ID; }
      inline const std::string& GetName() const { return Name; }

    private:
      GLuint ID, Width, Height;
      std::string Name;
      void load(const char* texPath);
  };
} }

