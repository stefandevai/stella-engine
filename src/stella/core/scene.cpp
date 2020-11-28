#include "stella/core/scene.hpp"
#include "stella/system_tags.hpp"
#include "stella/systems/render2.hpp"
#include "stella/systems/animation_player.hpp"
#include <stdexcept>
#include <spdlog/spdlog.h>
#include <iostream>

// TEMP
#include "stella/components/mesh.hpp"
#include "stella/components/position2.hpp"
#include "stella/components/sprite2.hpp"
#include "stella/components/animation_player.hpp"
// TEMP

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

  void Scene::start()
  {
    m_script_api.load ("/Users/stefandevai/Developer/games/stella-engine/nikte2/scripts/main.lua");

    auto mesh_entity = m_registry.create();
    m_registry.emplace<component::Mesh> (mesh_entity, "skull");
    m_registry.emplace<component::Position2> (mesh_entity, 0.0f, 0.0f);

    auto sprite_entity = m_registry.create();
    m_registry.emplace<component::Sprite> (sprite_entity, "spritesheet-nikte", 0);
    m_registry.emplace<component::Position2> (sprite_entity, 200.0f, 400.0f);
    m_registry.emplace<component::AnimationPlayer> (sprite_entity);
    auto& animations = m_registry.get<component::AnimationPlayer> (sprite_entity);
    animations.add ("idle-down", component::AnimationData{{0}, 0.1f});
    animations.add ("idle-right", component::AnimationData{{18}, 0.1f});
    animations.add ("idle-up", component::AnimationData{{27}, 0.1f});
    animations.add ("idle-left", component::AnimationData{{45}, 0.1f});
    animations.add ("walk-right", component::AnimationData{{19, 20, 21, 22, 23, 24, 25, 26}, 0.1f, true});
    animations.current = "walk-right";

    auto sprite_entity2 = m_registry.create();
    m_registry.emplace<component::Sprite> (sprite_entity2, "tileset");
    m_registry.emplace<component::Position2> (sprite_entity2, 0.0f, 0.0f);
  }

  void Scene::update (const double dt)
  {
    m_camera.update (m_registry);

    if (m_systems["animation"] != nullptr)
    {
      auto animation_system = std::dynamic_pointer_cast<system::AnimationPlayer> (m_systems.at ("animation"));
      animation_system->update(m_registry, dt);
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
