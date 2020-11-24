#pragma once

#include "stella/systems/system.hpp"
#include "stella/core/json.hpp"
#include "stella/graphics/camera.hpp"
#include <string>
#include <vector>
#include <memory>
#include <entt/entity/registry.hpp>

#ifdef STELLA_BUILD_EDITOR
namespace editor
{
  class Editor;
}

namespace editor
{
namespace widget
{
  struct SceneEditor;
  struct NewSystem;
}
}
#endif

namespace stella
{
namespace core
{
  using SystemMap = std::unordered_map<std::string, std::shared_ptr<system::System>>;
  class AssetManager;

  class Scene
  {
  public:
    Scene (const int width, const int height, AssetManager& asset_manager);

    void load (const std::string& filepath);
    void save (const std::string& filepath);
    void save();
    void start();
    void update (const double dt);
    void render (const double dt);
    void add_system (const std::string& system_name);
    inline bool is_modified() const { return m_modified; };
    inline std::string get_name() const { return m_name; };
    inline std::string get_filepath() const { return m_filepath; };
    inline void set_name (const std::string& name) { m_name = name; m_modified = true; };
    inline void set_filepath (const std::string& filepath) { m_filepath = filepath; m_modified = true; };

#ifdef STELLA_BUILD_EDITOR
  friend class ::editor::Editor;
  friend struct ::editor::widget::SceneEditor;
  friend struct ::editor::widget::NewSystem;
#endif

  private:
    JSON m_json;
    std::string m_name;
    std::string m_filepath;
    const int m_width;
    const int m_height;
    AssetManager& m_asset_manager;
    entt::registry m_registry;
    graphics::Camera m_camera;
    bool m_modified = false;

    // Storing systems in this way allow us to render them in order
    //std::vector<std::shared_ptr<system::System>> m_systems;
    SystemMap m_systems =
    {
      { "render", nullptr },
      { "animation", nullptr }
    };

  private:
    template<typename T, typename... Params>
    void m_add_system (const std::string& name, Params&... params)
    {
      m_systems[name] = std::make_shared<T> (params...);
    }
  };

} // namespace core
} // namespace stella
