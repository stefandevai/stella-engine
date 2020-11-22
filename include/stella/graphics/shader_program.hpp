#pragma once

#include "stella/graphics/opengl.hpp"
#include "stella/core/asset.hpp"
#include <glm/glm.hpp>

namespace stella
{
namespace graphics
{
  class ShaderProgram : public core::Asset
  {
  public:
    ShaderProgram(const char* vertex_source, const char* fragment_source);
    ~ShaderProgram();

    void use();
    void set_mat_4 (const std::string& uniform_name, const glm::mat4 &mat);

  private:
    int m_program;
  };


}
}
