#include "shader.h"
#include "../utils/fileutils.h"

namespace stella { namespace graphics {
  Shader::Shader(const char* vsPath, const char* fsPath)
    : VsPath(vsPath), FsPath(fsPath)
  {
    this->ShaderProgram = this->load();
  }

  Shader::~Shader()
  {
    glDeleteProgram(this->ShaderProgram);
  }

  void Shader::Enable()
  {
    glUseProgram(this->ShaderProgram); 
  }

  void Shader::Disable()
  {
    glUseProgram(0);
  }

  GLuint Shader::load()
  {
    std::string vertexSourceStr = read_file(this->VsPath); 
    const char* vertexSource = vertexSourceStr.c_str(); 
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, nullptr);
    glCompileShader(vertexShader);

    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
      glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
      std::cout << "Vertex Shader compilation failed!\n" << infoLog << std::endl;
    }

    std::string fragmentSourceStr = read_file(this->FsPath);
    const char* fragmentSource = fragmentSourceStr.c_str(); 
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
      glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
      std::cout << "Fragment Shader compilation failed!\n" << infoLog << std::endl;
    }

    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &success);

    if (!success)
    {
      glGetProgramInfoLog(program, 512, nullptr, infoLog);
      std::cout << "Program linking failed!\n" << infoLog << std::endl;
    }

    glDeleteShader(fragmentShader);
    glDeleteShader(vertexShader);
    
    return program;
  }
} }

