#pragma once

#include <string>
#include <vector>
#include <memory>
#include <entt/entity/registry.hpp>
#include "stella/systems/system.hpp"
#include "stella/core/json.hpp"

#ifdef STELLA_BUILD_EDITOR
namespace stella
{
namespace editor
{
  class Editor;
}
namespace widget
{
  struct SceneEditor;
}
} // namespace stella
#endif

namespace stella
{
namespace core
{
  class Scene
  {
  public:
    Scene ();

    void load(const std::string& filepath);
    void save(const std::string& filepath);
    void start();
    void update(const double dt);
    void render(const double dt);
    void update_systems (const double dt);
    inline std::string get_name() const { return m_name; };
    inline std::string get_filepath() const { return m_filepath; };
    inline void set_name(const std::string& name) { m_name = name; };
    inline void set_filepath(const std::string& filepath) { m_filepath = filepath; };

#ifdef STELLA_BUILD_EDITOR
  friend class stella::editor::Editor;
  friend struct stella::widget::SceneEditor;
#endif

  private:
    JSON m_json;
    std::string m_name;
    std::string m_filepath;
    std::vector<std::shared_ptr<system::System>> m_systems;
    entt::registry m_registry;
  };

} // namespace core
} // namespace stella
