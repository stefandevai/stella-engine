#pragma once

#include <GL/glew.h>
#include "../../glm/glm/glm.hpp"

namespace stella { namespace graphics {
  class Shader
  {
    public:
      Shader();
      ~Shader();
    private:
      GLuint Program;
  };
} }

