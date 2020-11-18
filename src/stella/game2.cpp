#include "stella/game2.hpp"
#include "stella/components/position.hpp"

namespace stella
{
  Game::Game (const std::string& script_path)
    : m_script_path (script_path)
  {
    m_json.set_filepath(m_script_path);
    m_json.load();
    //m_lua.script_file(m_script_path);
    m_init_variables();
    //m_init_scenes();
  }

  void Game::add_scene(std::shared_ptr<core::Scene>& scene)
  {
    m_scenes.push_back(scene);
  }

  // Syntatic sugar for scene creation
  void Game::create_scene(const std::string& name, const std::string& script_path)
  {
    auto scene = std::make_shared<core::Scene>(script_path);
    add_scene(scene);
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

  void Game::m_init_variables()
  {
    auto game_object = m_json.object["game"];
    if (game_object == nullptr)
    {
      std::cout << "[x] No game object was found on config file.\n";
      return;
    }

    if (game_object["width"] != nullptr && game_object["height"] != nullptr)
    {
      auto width = game_object["width"].get<int>();
      auto height = game_object["height"].get<int>();
      m_display.set_size(width, height);
    }

    if (game_object["title"] != nullptr)
    {
      auto title = game_object["title"].get<std::string>();
      m_display.set_title(title);
    }
  }

  void Game::m_init_scenes()
  {
    if (m_lua["scenes"] == sol::lua_nil)
    {
      return;
    }
    auto scenes = m_lua["scenes"].get<std::vector<std::string>>();

    for (auto& scene_path : scenes)
    {
      auto scene = std::make_shared<core::Scene>(scene_path);
      add_scene(scene);
    }
  }
}

