#pragma once

#include "stella/graphics/opengl.hpp"
#include "stella/core/asset.hpp"

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

  private:
    int m_program;
  };


}
}
