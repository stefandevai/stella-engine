#include "stella/application.hpp"
#include <iostream>

namespace stella
{
namespace core
{
  Application::Application (unsigned width, unsigned height, const std::string& title)
    : m_initial_width (width), m_initial_height (height), m_initial_title (title)
  {
    m_display.SetClearColor (0.0f, 0.0f, 0.0f);
    srand (time (nullptr));
  }

  Application::~Application() {}

  void Application::run()
  {
    while (m_display.IsRunning())
    {
      m_display.Clear();
      this->update (m_display.GetDT());
      m_display.Update();
    }
  }
} // namespace core
} // namespace stella
