#pragma once

#include <GL/glew.h>
#include "../../Dependencies/glm/glm/glm.hpp"

namespace stella { namespace graphics {
  class Shader
  {
    public:
      Shader(const char* vsPath, const char* fsPath);
      ~Shader();

      void Enable();
      void Disable();

      // Uniform setting
      void SetFloat(const GLchar *uniformName, const GLfloat &num);
      void SetInt(const GLchar *uniformName, const GLint &num);
      void SetIntv(const GLchar *uniformName, GLint *array, int count);
      void SetVec2f(const GLchar *uniformName, const glm::vec2 &vec);
      void SetVec2f(const GLchar *uniformName, const GLfloat &x, const GLfloat &y);
      void SetVec3f(const GLchar *uniformName, const glm::vec3 &vec);
      void SetVec3f(const GLchar *uniformName, const GLfloat &x, const GLfloat &y, const GLfloat &z);
      void SetVec4f(const GLchar *uniformName, const glm::vec4 &vec);
      void SetVec4f(const GLchar *uniformName, const GLfloat &x, const GLfloat &y, const GLfloat &z, const GLfloat &w);
      void SetMat4(const GLchar *uniformName, const glm::mat4 &mat);
    private:
      GLuint ShaderProgram;
      const char *VsPath, *FsPath;

      GLuint load();
  };
} }
