#include "stella/application.hpp"
#include <ctime>

namespace stella
{
namespace core
{
  Application::Application (unsigned width, unsigned height, const std::string& title) : m_initial_width (width), m_initial_height (height), m_initial_title (title)
  {
    m_display.set_clear_color (0.0f, 0.0f, 0.0f);
    srand (std::time (nullptr));
  }

  Application::~Application() {}

  void Application::run()
  {
    while (m_display.is_running())
    {
      m_display.clear();
      this->update (m_display.get_dt());
      this->render (m_display.get_dt());
      m_display.update();
    }
  }
} // namespace core
} // namespace stella
