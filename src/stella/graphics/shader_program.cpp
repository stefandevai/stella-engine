#include "stella/graphics/shader_program.hpp"
#include "stella/graphics/opengl.hpp"
#include <spdlog/spdlog.h>

namespace stella
{
namespace graphics
{
  ShaderProgram::ShaderProgram (const char* vertex_source, const char* fragment_source)
  {
    int vertex_shader = glCreateShader (GL_VERTEX_SHADER);
    glShaderSource (vertex_shader, 1, &vertex_source, NULL);
    glCompileShader (vertex_shader);

    int success;
    char info_log[512];
    glGetShaderiv (vertex_shader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
      glGetShaderInfoLog (vertex_shader, 512, NULL, info_log);
      spdlog::critical ("Vertex Shader compilation failed: {}\n{}", vertex_source, info_log);
    }

    int fragment_shader = glCreateShader (GL_FRAGMENT_SHADER);
    glShaderSource (fragment_shader, 1, &fragment_source, NULL);
    glCompileShader (fragment_shader);

    glGetShaderiv (fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
      glGetShaderInfoLog (fragment_shader, 512, NULL, info_log);
      spdlog::critical ("Fragment Shader compilation failed: {}\n{}", fragment_source, info_log);
    }

    m_program = glCreateProgram();
    glAttachShader (m_program, vertex_shader);
    glAttachShader (m_program, fragment_shader);
    glLinkProgram (m_program);

    glGetProgramiv (m_program, GL_LINK_STATUS, &success);
    if (!success)
    {
      glGetProgramInfoLog (m_program, 512, NULL, info_log);
      spdlog::critical ("Program linking failed: {} {}\n{}", vertex_source, fragment_source, info_log);
    }
    glDeleteShader (vertex_shader);
    glDeleteShader (fragment_shader);
  }

  ShaderProgram::~ShaderProgram() { glDeleteProgram (m_program); }

  void ShaderProgram::use() { glUseProgram (m_program); }

  void ShaderProgram::set_mat_4 (const std::string& uniform_name, const glm::mat4& mat) const
  {
    glUniformMatrix4fv (glGetUniformLocation (m_program, uniform_name.c_str()), 1, GL_FALSE, &mat[0][0]);
  }

  void ShaderProgram::set_float (const std::string& uniform_name, const float value) const { glUniform1f (glGetUniformLocation (m_program, uniform_name.c_str()), value); }

  void ShaderProgram::set_int (const std::string& uniform_name, const int value) const { glUniform1i (glGetUniformLocation (m_program, uniform_name.c_str()), value); }
} // namespace graphics
} // namespace stella
