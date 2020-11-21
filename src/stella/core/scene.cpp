#include "stella/core/scene.hpp"
#include "stella/system_tags.hpp"
#include "stella/systems/render.hpp"
#include "stella/systems/animation_player.hpp"
#include <stdexcept>
#include <spdlog/spdlog.h>
#include <iostream>

namespace stella
{
namespace core
{
  Scene::Scene ()
  {
    spdlog::set_level(spdlog::level::debug);
  }

  void Scene::load(const std::string& filepath)
  {
    m_json.load(filepath);

    if (m_json.object["name"] == nullptr)
    {
      throw std::invalid_argument("No Scene name was provided.");
    }

    if (m_json.object["systems"] != nullptr)
    {
      auto system_names = m_json.object["systems"].get<std::vector<std::string>>();
      for (auto& system_name : system_names)
      {
        add_system(system_name);
      }
    }

    m_name = m_json.object["name"].get<std::string>();
    m_filepath = filepath;
  }

  void Scene::save(const std::string& filepath)
  {
    if (filepath.empty())
    {
      throw std::invalid_argument("Filepath is empty.");
    }
    if (m_name.empty())
    {
      throw std::invalid_argument("Scene name should not be empty.");
    }
    m_json.object["name"] = m_name;
    m_json.save(filepath);
    m_filepath = filepath;
    m_modified = false;
  }

  void Scene::save()
  {
    save(m_filepath);
  }

  void Scene::start() {}

  void Scene::update (const double dt)
  {
    m_update_systems(dt);
  }

  void Scene::render (const double dt) {}

  void Scene::add_system (const std::string& system_name)
  {
    SystemTag system_tag;

    try
    {
      system_tag = g_system_map.at(system_name);
    }
    catch (std::out_of_range& e)
    {
      spdlog::warn("Unknown system: {}\n{}", system_name, e.what());
      return;
    }
    
    switch (system_tag)
    {
      case SystemTag::RENDER:
        {
          m_add_system<stella::system::RenderT>(m_registry, m_textures);
        }
        break;

      case SystemTag::ANIMATION:
        {
          m_add_system<stella::system::AnimationPlayer>();
        }
        break;

      default:
        {
          spdlog::warn("Unknown system: {}", system_name);
        }
      break;
    }
  }

  void Scene::m_update_systems (const double dt)
  {
    for (auto& s : m_systems)
    {
      s->update (m_registry, dt);
    }
  }
} // namespace core
} // namespace stella
