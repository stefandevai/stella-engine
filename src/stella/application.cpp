#include "stella/application.h"
#include <iostream>

namespace stella
{
namespace core
{
  Application::Application (unsigned width, unsigned height, const std::string& title)
    : m_initial_width (width), m_initial_height (height), m_initial_title (title)
  {
    m_display.SetClearColor (0.3f, 0.3f, 0.3f);
    srand (time (nullptr));
  }

  Application::~Application() {}

  void Application::run()
  {
    while (m_display.IsRunning())
    {
      m_display.Clear();
      this->update (m_display.GetDT());
#ifdef STELLA_BUILD_EDITOR
      m_display.UpdateEditor (m_registry);
#endif
      m_display.Update();
    }
  }
} // namespace core
} // namespace stella
