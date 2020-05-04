#include "editor/widgets/component_menu.hpp"

namespace stella
{
namespace widget
{

  ComponentMenu::ComponentMenu ()
  : Widget ("Component Menu")
  {
      reset();
  }

  void ComponentMenu::render ()
  {
      ImGui::Text("Select Components:");
      ImGui::Dummy (ImVec2 (0.f, 10.f));
      for (const auto& component : ComponentList::component_map)
      {
        ImGui::PushID((component.second + "#component-menu").c_str());
        if (ImGui::MenuItem(component.second.c_str(), "", &m_selected_components[component.first])) { }
        ImGui::PopID();
      }
  }

  void ComponentMenu::reset()
  {
      for (const auto& component : ComponentList::component_map)
      {
          m_selected_components[component.first] = false;
      }
  }

} // namespace widget
} // namespace stella

