#include "stella/graphics/framebuffer.hpp"
#include "stella/graphics/shader.hpp"

#include "stella/graphics/opengl.hpp" // IWYU pragma: export

#include <spdlog/spdlog.h>

namespace stella
{
namespace graphics
{
  Framebuffer::Framebuffer (stella::graphics::Display& display) : m_display (display) { m_init(); }

  Framebuffer::~Framebuffer()
  {
    if (m_fbo)
    {
      glDeleteFramebuffers (1, &m_fbo);
    }
  }

  void Framebuffer::draw()
  {
    m_display.set_clear_color (0.f, 0.f, 0.f);
    m_display.clear();
    glBindVertexArray (m_vao);
    glBindTexture (GL_TEXTURE_2D, get_texture());
    glDrawArrays (GL_TRIANGLES, 0, 6);
  }

  void Framebuffer::bind()
  {
    float TexW = m_current_texture_resolution.x;
    float TexH = m_current_texture_resolution.y;

    // Updates FBO's texture resolution on window resizing
    if (TexW != m_display.get_width() || TexH != m_display.get_height())
    {
      m_refresh_texture_resolution();
    }

    glBindFramebuffer (GL_FRAMEBUFFER, m_fbo);
  }

  void Framebuffer::m_refresh_texture_resolution()
  {
    m_current_texture_resolution = glm::vec2 (m_display.get_width(), m_display.get_height());

    glBindTexture (GL_TEXTURE_2D, m_fbo_texture);
    glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA, m_current_texture_resolution.x, m_current_texture_resolution.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glBindTexture (GL_TEXTURE_2D, 0);
  }

  void Framebuffer::activate_texture (GLenum texture_id)
  {
    glActiveTexture (texture_id);
    glBindTexture (GL_TEXTURE_2D, get_texture());
  }

  void Framebuffer::unbind() { glBindFramebuffer (GL_FRAMEBUFFER, 0); }

  void Framebuffer::m_init()
  {
    m_current_texture_resolution = glm::vec2 (m_display.get_width(), m_display.get_height());

    // Framebuffer
    glGenFramebuffers (1, &m_fbo);
    glBindFramebuffer (GL_FRAMEBUFFER, m_fbo);

    glGenTextures (1, &m_fbo_texture);
    glBindTexture (GL_TEXTURE_2D, m_fbo_texture);
    glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA, m_current_texture_resolution.x, m_current_texture_resolution.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture (GL_TEXTURE_2D, 0);

    glFramebufferTexture2D (GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_fbo_texture, 0);

    unsigned int RBO;
    glGenRenderbuffers (1, &RBO);
    glBindRenderbuffer (GL_RENDERBUFFER, RBO);
    glRenderbufferStorage (GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_current_texture_resolution.x, m_current_texture_resolution.y);
    glBindRenderbuffer (GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer (GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

    if (glCheckFramebufferStatus (GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
      spdlog::critical ("Framebuffer is not complete.");
    }

    glBindFramebuffer (GL_FRAMEBUFFER, 0);

    // Quad
    float vertices[] = {-1.0f, 1.0f, 0.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 1.0f, 0.0f, 1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f};
    glGenVertexArrays (1, &m_vao);
    glGenBuffers (1, &m_vbo);
    glBindVertexArray (m_vao);
    glBindBuffer (GL_ARRAY_BUFFER, m_vbo);
    glBufferData (GL_ARRAY_BUFFER, sizeof (vertices), &vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray (0);
    glVertexAttribPointer (0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof (float), (void*) 0);
    glEnableVertexAttribArray (1);
    glVertexAttribPointer (1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof (float), (void*) (2 * sizeof (float)));
  }
} // namespace graphics
} // namespace stella
