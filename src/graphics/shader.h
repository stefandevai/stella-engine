#pragma once

#include <GL/glew.h>
#include "../../glm/glm/glm.hpp"

namespace stella { namespace graphics {
  class Shader
  {
    public:
      Shader(const char* vsPath, const char* fsPath);
      ~Shader();

      void Enable();
      void Disable();
    private:
      GLuint ShaderProgram;
      const char *VsPath, *FsPath;

      GLuint load();
  };
} }

