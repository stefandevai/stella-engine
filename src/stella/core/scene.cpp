#include "stella/core/scene.hpp"
#include "stella/system_tags.hpp"
#include "stella/systems/render2.hpp"
#include "stella/systems/animation_player.hpp"
#include "stella/systems/text.hpp"
#include <stdexcept>
#include <spdlog/spdlog.h>
#include <iostream>

namespace stella
{
namespace core
{
  Scene::Scene (const int width, const int height, AssetManager& asset_manager) : m_width (width), m_height (height), m_asset_manager (asset_manager)
  {
    spdlog::set_level (spdlog::level::debug);
    m_camera.set_frustrum (0.0f, static_cast<float> (m_width), static_cast<float> (m_height), 0.0f);
  }

  void Scene::load (const std::string& filepath)
  {
    m_json.load (filepath);

    if (m_json.object["name"] == nullptr)
    {
      throw std::invalid_argument ("No Scene name was provided.");
    }

    if (m_json.object["systems"] != nullptr)
    {
      auto system_names = m_json.object["systems"].get<std::vector<std::string>>();
      for (auto& system_name : system_names)
      {
        add_system (system_name);
      }
    }

    m_name     = m_json.object["name"].get<std::string>();
    m_filepath = filepath;

    // Scene is not modified when loaded
    m_modified = false;
  }

  void Scene::save (const std::string& filepath)
  {
    if (filepath.empty())
    {
      throw std::invalid_argument ("Filepath is empty.");
    }
    if (m_name.empty())
    {
      throw std::invalid_argument ("Scene name should not be empty.");
    }

    // Update name
    m_json.object["name"] = m_name;

    // Update system tags
    m_json.object["systems"] = nlohmann::json::array();

    for (auto& s : m_systems)
    {
      if (s.second != nullptr)
      {
        m_json.object["systems"].emplace_back (s.second->get_tag());
      }
    }

    // Save json file
    m_json.save (filepath);
    m_filepath = filepath;
    m_modified = false;
  }

  void Scene::save() { save (m_filepath); }

  /* void Scene::start() { m_script_api.load ("/Users/stefandevai/Developer/games/stella-engine/example/scripts/main.lua"); } */
  void Scene::start() { m_script_api.load ("./scripts/main.lua"); }

  void Scene::update (const double dt)
  {
    m_camera.update (m_registry);

    if (m_systems["animation"] != nullptr)
    {
      auto animation_system = std::dynamic_pointer_cast<system::AnimationPlayer> (m_systems.at ("animation"));
      animation_system->update (m_registry, dt);
    }

    if (m_systems["text"] != nullptr)
    {
      auto text_system = std::dynamic_pointer_cast<system::Text> (m_systems.at ("text"));
      text_system->update (m_registry, dt);
    }
  }

  void Scene::render (const double dt)
  {
    auto render_system = std::dynamic_pointer_cast<system::Render> (m_systems.at ("render"));

    if (render_system != nullptr)
    {
      render_system->render (m_registry, m_camera, dt);
    }
  }

  void Scene::add_system (const std::string& system_name)
  {
    SystemTag system_tag;

    try
    {
      system_tag = g_system_map.at (system_name);
    }
    catch (std::out_of_range& e)
    {
      spdlog::warn ("Unknown system: {}\n{}", system_name, e.what());
      return;
    }

    m_modified = true;

    switch (system_tag)
    {
      case SystemTag::RENDER:
      {
        m_add_system<stella::system::Render> (system_name, m_asset_manager);
      }
      break;

      case SystemTag::ANIMATION:
      {
        m_add_system<stella::system::AnimationPlayer> (system_name);
      }
      break;

      case SystemTag::TEXT:
      {
        m_add_system<stella::system::Text> (system_name, m_registry, m_asset_manager);
      }
      break;

      default:
      {
        spdlog::warn ("Unknown system: {}", system_name);
        m_modified = false;
      }
      break;
    }
  }
} // namespace core
} // namespace stella
