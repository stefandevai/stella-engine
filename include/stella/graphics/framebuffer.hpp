#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "stella/graphics/display.hpp"

typedef unsigned int GLuint;
typedef unsigned int GLenum;

namespace stella
{
namespace graphics
{
  class Framebuffer
  {
  public:
    explicit Framebuffer (stella::graphics::Display& display);
    ~Framebuffer();

    void bind();
    static void unbind();
    void draw();

    void activate_texture (GLenum texture_id);
    inline GLuint get_texture() const { return m_fbo_texture; }

  private:
    GLuint m_fbo, m_fbo_texture, m_vao, m_vbo;
    stella::graphics::Display& m_display;
    glm::vec2 m_current_texture_resolution;

    void m_init();
    void m_refresh_texture_resolution();
  };
} // namespace graphics
} // namespace stella
