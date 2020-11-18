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

  void Game::add_scene(std::shared_ptr<core::Scene>& scene)
  {
    m_scenes.push_back(scene);
  }

  // Syntatic sugar for scene creation
  void Game::create_scene(const std::string& name, const std::string& script_path)
  {
    auto scene = std::make_shared<core::Scene>(name, script_path);
    add_scene(scene);
    load_scene(name);
  }

  void Game::load_scene(const std::string& name)
  {
    auto scene_it = std::find_if(m_scenes.begin(), m_scenes.end(),
        [name](auto& scene)
        {
          return (scene->get_name() == name);
        });

    // Scene not found
    if (scene_it == m_scenes.end())
    {
      return;
    }

    m_current_scene = *scene_it;
    m_current_scene->load();
  }

  void Game::update(const double dt)
  {
    // No scene is loaded yet
    if (m_current_scene == nullptr)
    {
      return;
    }

    m_current_scene->update(dt);
  }

  void Game::render(const double dt)
  {
    // No scene is loaded yet
    if (m_current_scene == nullptr)
    {
      return;
    }

    m_current_scene->render(dt);
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
}

