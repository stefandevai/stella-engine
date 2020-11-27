#pragma once

#include "stella/graphics/display.hpp"
#include "stella/core/scene.hpp"
#include "stella/core/json.hpp"
#include "stella/core/resource/asset_manager.hpp"

#include <entt/entity/registry.hpp>

#include <string>
#include <vector>
#include <memory>
#include <filesystem>

namespace stella
{
class Game
{
public:
  Game (const std::filesystem::path& filepath);

  void run();
  inline const uint_fast32_t get_width() const { return m_display.get_width(); };
  inline const uint_fast32_t get_height() const { return m_display.get_height(); };
  inline void set_size (const int width, const int height)
  {
    m_width  = width;
    m_height = height;
    m_display.set_size (width, height);
  };
  inline void set_title (const std::string& title)
  {
    m_title = title;
    m_display.set_title (title);
  };
  // inline void set_filepath(const std::string& filepath) { m_filepath = filepath; };

  void add_scene (std::shared_ptr<core::Scene>& scene);
  void load_scene (const std::string& filepath);
  void create_scene (const std::string& name, const std::string& filepath);
  void start_scene (const std::string& name);
  void start_current_scene();
  void save();
  void update (const double dt);
  void render (const double dt);
  void quit();

private:
  // Hardcoded fields
  std::string m_initial_title = "Stella Engine";
  unsigned m_width            = 800;
  unsigned m_height           = 600;
  const std::filesystem::path m_config_filepath{"config.json"};
  const std::filesystem::path m_assets_config_filepath{"assets/assets.json"};
  const std::filesystem::path m_scenes_dir{"scenes"};

  // General fields
  const std::filesystem::path m_filepath;
  core::JSON m_json;
  core::AssetManager m_asset_manager{m_filepath / m_assets_config_filepath};
  entt::registry m_registry;
  std::string m_title;
  graphics::Display m_display{m_width, m_height, m_initial_title};

  // TODO: Use another data structure to hold scenes and names
  std::vector<std::shared_ptr<core::Scene>> m_scenes;
  std::shared_ptr<core::Scene> m_current_scene = nullptr;

#ifdef STELLA_BUILD_EDITOR
  friend class ::editor::Editor;
  friend struct ::editor::widget::SceneEditor;
#endif

private:
  void m_init_variables();
  void m_init_scenes();
};
} // namespace stella
