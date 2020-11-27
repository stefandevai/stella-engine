#pragma once

#include "stella/graphics/opengl.hpp"
#include "stella/core/resource/asset.hpp"
#include <glm/glm.hpp>

namespace stella
{
namespace graphics
{
  class ShaderProgram : public core::Asset
  {
  public:
    ShaderProgram (const char* vertex_source, const char* fragment_source);
    ~ShaderProgram();

    void use();
    void set_float (const std::string& uniform_name, const float value) const;
    void set_mat_4 (const std::string& uniform_name, const glm::mat4& mat) const;

  private:
    int m_program;
  };

} // namespace graphics
} // namespace stella
