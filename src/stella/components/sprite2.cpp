#include "stella/components/sprite2.hpp"

namespace stella::component
{
const glm::vec2 Sprite::get_size() const
{
  if (texture == nullptr)
  {
    throw std::runtime_error ("Texture has not been initialized.");
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

  return texture->get_frame_coords (m_frame);
}

} // namespace stella::component
