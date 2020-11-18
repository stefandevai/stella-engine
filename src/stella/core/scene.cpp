#include "stella/core/scene.hpp"

namespace stella
{
namespace core
{
  Scene::Scene (const std::string& script_path)
    : m_script_path (script_path)
  {
    m_lua.open_libraries (sol::lib::base);
    m_lua.script_file(m_script_path);

    // TODO: Error handling when no scene object is provided
    if (m_lua["scene"] != sol::lua_nil && m_lua["scene"]["name"] != sol::lua_nil)
    {
      m_name = m_lua["scene"]["name"].get<std::string>();
    }
  }

  Scene::~Scene() {}

  void Scene::load() {}

  void Scene::update(const double dt) {}

  void Scene::render(const double dt) {}

  void Scene::update_systems (const double dt)
  {
    for (auto& s : m_systems)
    {
      s->update (m_registry, dt);
    }
  }
} // namespace core
} // namespace stella
