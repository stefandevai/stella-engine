#pragma once

#include "stella/graphics/display.hpp"
#include <entt/entity/registry.hpp>
#include <string>

// #ifdef STELLA_BUILD_EDITOR
//   #include "editor/editor.hpp"
  
// #endif

#ifdef STELLA_BUILD_EDITOR
namespace stella
{
namespace editor
{
  class Editor;
}
} // namespace stella
#endif

namespace stella
{
namespace core
{
  class Application
  {
  protected:
    entt::registry m_registry;
    const unsigned m_initial_width, m_initial_height;
    const std::string& m_initial_title;
    graphics::Display m_display{m_initial_width, m_initial_height, m_initial_title};

  public:
    Application (const unsigned width, const unsigned height, const std::string& title);
    virtual ~Application();
    void run();
    inline const uint_fast32_t width() const { return m_display.GetWidth(); };
    inline const uint_fast32_t height() const { return m_display.GetHeight(); };

  protected:
    virtual void load()                   = 0;
    virtual void update (const double dt) = 0;

#ifdef STELLA_BUILD_EDITOR
    friend class editor::Editor;
#endif
  };

} // namespace core
} // namespace stella
