#include "stella/components/sprite2.hpp"

namespace stella::component
{
const glm::vec2 Sprite::get_size() const
{
  if (texture == nullptr)
  {
    throw std::runtime_error ("Texture has not been initialized.");
  }

  if (m_has_custom_uv)
  {
    return glm::vec2 (m_custom_width, m_custom_height);
  }

  return glm::vec2 (texture->get_frame_width (m_frame), texture->get_frame_height (m_frame));
}

// Get top-left, top-right, bottom-right and bottom-left uv coordinates
const std::array<glm::vec2, 4> Sprite::get_texcoords() const
{
  if (texture == nullptr)
  {
    throw std::runtime_error ("Texture has not been initialized.");
  }

  if (m_has_custom_uv)
  {
    const auto width  = texture->get_frame_width (m_frame);
    const auto height = texture->get_frame_height (m_frame);

    const auto top    = m_custom_height / height;
    const auto bottom = m_custom_uv.y / height;
    const auto right  = m_custom_width / width;

    return std::array<glm::vec2, 4>{
        glm::vec2{m_custom_uv.x, bottom - top},
        glm::vec2{m_custom_uv.x + right, bottom - top},
        glm::vec2{m_custom_uv.x + right, bottom},
        glm::vec2{m_custom_uv.x, bottom},
    };
  }

  return texture->get_frame_coords (m_frame);
}

void Sprite::set_custom_uv (const float left, const float top, const float width, const float height)
{
  m_custom_uv     = glm::vec2 (left, top);
  m_custom_width  = width;
  m_custom_height = height;
  m_has_custom_uv = true;
}

} // namespace stella::component
