#include "stella/core/scene.hpp"

namespace stella
{
namespace core
{
  Scene::Scene (const std::string& name, const std::string& script_path) : m_name (name), m_script_path (script_path) {}

  Scene::~Scene() {}

  void Scene::load() {}

  void Scene::update() {}

  void Scene::update_systems (const double dt)
  {
    for (auto& s : m_systems)
    {
      s->update (m_registry, dt);
    }
  }

  void Scene::render() {}

} // namespace core
} // namespace stella
