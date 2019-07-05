#pragma once

#include <string>
#include "../graphics/display.h"

namespace stella
{
namespace core
{

  class Application
  {
    protected:
      const unsigned m_initial_width, m_initial_height;
      const std::string &m_initial_title;
      graphics::Display m_display{m_initial_width, m_initial_height, m_initial_title};

    public:
      Application(const unsigned width, const unsigned height, const std::string &title);
      virtual ~Application();
      void run();
      inline uint_fast32_t width();
      inline uint_fast32_t height();

    protected:
      virtual void load() = 0;
      virtual void update(const double dt) = 0;
  };

} // namespace script
} // namespace st
