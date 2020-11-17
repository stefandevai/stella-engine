#pragma once

#include <string>
#include <entt/entity/registry.hpp>
#include "stella/graphics/display.hpp"
#include "stella/graphics/font.hpp"
#include "stella/types.hpp"

namespace stella
{
  class Game
  {
  public:
    Game (const unsigned width, const unsigned height, const std::string& title);
    
    void run();
    //std::vector<float> get_camera_pos();
    inline const uint_fast32_t width() const { return m_display.get_width(); };
    inline const uint_fast32_t height() const { return m_display.get_height(); };

  private:
    void load();
    void update (const double dt);
    void render (const double dt);

  private:
    entt::registry m_registry;
    const unsigned m_initial_width, m_initial_height;
    const std::string& m_initial_title;
    graphics::Display m_display{m_initial_width, m_initial_height, m_initial_title};
    TextureManager m_textures;
    entt::registry::entity_type m_camera = m_registry.create();
    
#ifdef STELLA_BUILD_EDITOR
    friend class editor::Editor;
#endif
  };
}
