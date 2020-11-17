#pragma once

#include <string>
#include <vector>
#include <memory>
#include <entt/entity/registry.hpp>
#include "stella/systems/system.hpp"

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
    void update();
    void render();
    void update_systems (const double dt);

  private:
    const std::string& m_name;
    const std::string& m_script_path;
    std::vector<std::shared_ptr<system::System>> m_systems;
    entt::registry m_registry;
  };

} // namespace core
} // namespace stella
