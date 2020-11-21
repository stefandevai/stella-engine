#include "editor/widgets/new_system.hpp"
#include "stella/core/scene.hpp"
#include "stella/system_tags.hpp"

namespace editor
{
namespace widget
{
  NewSystem::NewSystem() : Widget ("new-system") {}

  std::string NewSystem::render (const std::shared_ptr<stella::core::Scene>& scene)
  {
    if (!m_open)
    {
      return "";
    }
    std::string system_tag{};
    std::vector<std::string> current_systems(stella::g_system_map.size());

    for (auto& system : scene->m_systems)
    {
      current_systems.emplace_back(system->get_tag());
    }

    if (ImGui::Begin("Add a System to the Current Scene", &m_open))
    {
      for (auto keypair : stella::g_system_map)
      {
        if (std::find(current_systems.begin(), current_systems.end(), keypair.first) == current_systems.end() &&
            ImGui::MenuItem(keypair.first.c_str(), "", false))
        {
          system_tag = keypair.first;
          m_open = false;
        }
      }
    }
    ImGui::End();

    return system_tag;
  }
} // namespace widget
} // namespace editor

