#include "stella/core/scene.hpp"
#include <stdexcept>

namespace stella
{
namespace core
{
  Scene::Scene () { }

  void Scene::load(const std::string& filepath)
  {
    m_json.load(filepath);

    if (m_json.object["name"] == nullptr)
    {
      throw std::invalid_argument("No Scene name was provided.");
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
