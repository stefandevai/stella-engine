#pragma once

#include "component.hpp"
#include "stella/graphics/texture2.hpp"
#include <glm/vec2.hpp>

namespace stella::component
{

  struct Sprite : public Component
  {
  public:
    std::string resource_id;
    std::shared_ptr<graphics::Texture> texture = nullptr;

  public:
    Sprite (const std::string& resource_id, const int frame = 0) : Component ("Sprite"), resource_id (resource_id), m_frame (frame) {}

    inline void set_frame (const int frame) { m_frame = frame; };
    inline void increment_frame () { ++m_frame; }
    inline void decrement_frame () { --m_frame; }
    const glm::vec2 get_size () const;
    // Get top-left, top-right, bottom-right and bottom-left uv coordinates
    const std::array<glm::vec2, 4> get_texcoords () const;

  private:
    int m_frame = 0;
  };

}
