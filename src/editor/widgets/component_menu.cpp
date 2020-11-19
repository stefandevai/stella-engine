#include "editor/widgets/component_menu.hpp"

namespace editor
{
namespace widget
{
  ComponentMenu::ComponentMenu() : Widget ("Component Menu") { reset(); }

  void ComponentMenu::render (entt::entity entity)
  {
    ImGui::Text ("Select Components:");
    ImGui::Dummy (ImVec2 (0.f, 10.f));
    ImGui::BeginChild ("ComponentList#3", ImVec2 (0, 200), true);
    for (const auto& component : stella::ComponentList::component_map)
    {
      ImGui::PushID ((component.second + "#component-menu").c_str());
      if (ImGui::MenuItem (component.second.c_str(), "", &m_selected_components[component.first])) {}
      ImGui::Dummy (ImVec2 (0.f, 5.f));
      ImGui::PopID();
    }
    ImGui::EndChild();
  }

  void ComponentMenu::reset()
  {
    for (const auto& component : stella::ComponentList::component_map)
    {
      m_selected_components[component.first] = false;
    }
  }

} // namespace widget
} // namespace editor
