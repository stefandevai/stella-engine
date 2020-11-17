#include "stella/game2.hpp"
#include "stella/components/position.hpp"

namespace stella
{
  Game::Game (unsigned width, unsigned height, const std::string& title)
    : m_initial_width (width), m_initial_height (height), m_initial_title (title)
  {
    m_display.set_clear_color (0.0f, 0.0f, 0.0f);
    srand (std::time (nullptr));
  }

  void Game::update(const double dt)
  {

  }

  void Game::render(const double dt)
  {

  }

  void Game::run()
  {
    while (m_display.is_running())
    {
      m_display.clear();
      this->update (m_display.get_dt());
      this->render (m_display.get_dt());
      m_display.update();
    }
  }

  //std::vector<float> Game::get_camera_pos()
  //{
    ////auto& pos = m_registry.get<stella::component::Position> (m_camera);
    ////return std::vector<float>{pos.x, pos.y, pos.z};
    //return std::vector<float>{0.f, 0.f, 0.f};
  //}
}

