#pragma once

#include <string>
#include <vector>
#include <memory>
#include <entt/entity/registry.hpp>
#include "stella/systems/system.hpp"

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
    Scene (const std::string& name, const std::string& script_path);
    ~Scene();

    void load();
    void update(const double dt);
    void render(const double dt);
    void update_systems (const double dt);
    inline std::string get_name() { return m_name; };
    inline std::string get_script_path() { return m_script_path; };

#ifdef STELLA_BUILD_EDITOR
  friend class stella::editor::Editor;
  friend struct stella::widget::SceneEditor;
#endif

  private:
    std::string m_name;
    std::string m_script_path;
    std::vector<std::shared_ptr<system::System>> m_systems;
    entt::registry m_registry;
  };

} // namespace core
} // namespace stella
