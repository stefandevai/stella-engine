#include "stella/game2.hpp"
#include "stella/graphics/texture2.hpp"
#include "stella/components/position.hpp"
#include <spdlog/spdlog.h>
#include <stdexcept>
#include <iostream>

namespace stella
{
  Game::Game (const std::filesystem::path& filepath)
    : m_filepath (filepath)
  {
    m_json.load(m_filepath / m_config_filepath);
    m_init_variables();
    m_init_scenes();
  }

  void Game::add_scene(std::shared_ptr<core::Scene>& scene)
  {
    m_scenes.push_back(scene);
  }

  void Game::load_scene(const std::string& filepath)
  {
    // TODO: Check if scene already exists
    // If not, add to config.json file
    auto scene = std::make_shared<core::Scene>();

    try
    {
      scene->load(filepath);
    }
    catch (std::invalid_argument& e)
    {
      spdlog::warn("Unable to load file: {}\n{}", filepath, e.what());
    }

    add_scene(scene);
    m_current_scene = scene;
  }

  void Game::create_scene(const std::string& name, const std::string& filepath)
  {
    // TODO: Check if scene with same filepath already exists
    auto scene = std::make_shared<core::Scene>();
    scene->set_name (name);
    scene->save (filepath);
    add_scene(scene);
    m_json.object["scenes"].emplace_back(std::filesystem::relative(filepath, m_filepath));
  }

  void Game::start_scene(const std::string& name)
  {
    auto scene_it = std::find_if(m_scenes.begin(), m_scenes.end(),
        [name](auto& scene)
        {
          return (scene->get_name() == name);
        });

    // Scene not found
    if (scene_it == m_scenes.end())
    {
      throw std::invalid_argument("Scene not found.");
    }

    m_current_scene = *scene_it;
    m_current_scene->start();
  }

  void Game::start_current_scene()
  {
    if (m_current_scene == nullptr)
    {
      return;
    }
    m_current_scene->start();
  }

  void Game::save()
  {
    if (m_filepath.empty())
    {
      spdlog::warn("Trying to save with an empty config filepath.");
      return;
    }
    m_json.object["game"] = nullptr;
    m_json.object["game"]["name"] = m_title;
    m_json.object["game"]["width"] = m_display.get_width();
    m_json.object["game"]["height"] = m_display.get_height();

    m_json.save(m_filepath / m_config_filepath);

    for (auto& scene : m_scenes)
    {
      if (scene->is_modified())
      {
        scene->save();
      }
    }
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
      update (m_display.get_dt());
      render (m_display.get_dt());
      m_display.update();
    }
  }

  void Game::quit()
  {
    m_display.quit();
  }

  void Game::m_init_variables()
  {
    auto game_object = m_json.object["game"];
    if (game_object == nullptr)
    {
      spdlog::warn("[x] No game object was found on config file.\n");
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
    if (m_json.object["scenes"] == nullptr)
    {
      return;
    }
    auto scenes = m_json.object["scenes"].get<std::vector<std::string>>();

    for (auto& scene_path : scenes)
    {
      load_scene(m_filepath / m_scenes_dir / scene_path);
    }

    if (m_json.object["game"]["firstScene"] == nullptr)
    {
      if (m_scenes.empty())
      {
        spdlog::warn("No scenes were loaded.");
        return;
      }

      start_current_scene();
      return;
    }

    auto first_scene = m_json.object["game"]["firstScene"].get<std::string>();
    try
    {
      start_scene (first_scene);
    }
    catch (std::invalid_argument& e)
    {
      spdlog::warn("Unable to load first scene: {} {}", first_scene, e.what());
    }
  }
}

