#include "stella/core/scene.hpp"
#include <iostream>

namespace stella
{
namespace core
{
  Scene::Scene () { }

  void Scene::load(const std::string& filepath)
  {
    m_filepath = filepath;
    m_json.load(m_filepath);

    // TODO: Error handling when no scene object is provided
    if (m_json.object["name"] != nullptr)
    {
      m_name = m_json.object["name"].get<std::string>();
    }
  }

  void Scene::save(const std::string& filepath)
  {
    m_json.object["name"] = m_name;
    m_json.save(filepath);
    m_filepath = filepath;
    m_modified = false;
  }

  void Scene::save()
  {
    if (m_filepath.empty())
    {
      return;
    }
    save(m_filepath);
  }

  void Scene::start() {}

  void Scene::update (const double dt) {}

  void Scene::render (const double dt) {}

  void Scene::update_systems (const double dt)
  {
    for (auto& s : m_systems)
    {
      s->update (m_registry, dt);
    }
  }
} // namespace core
} // namespace stella
