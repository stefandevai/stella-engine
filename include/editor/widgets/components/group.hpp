#pragma once

#include "stella/components/group.hpp"
#include "imgui.h"

namespace stella
{
namespace widget
{
  struct Group
  {
    Group (const std::vector<std::string>& texture_list) : m_texture_list (texture_list) {}
    void operator() (entt::registry& registry, const entt::entity entity)
    {
      ImGui::PushID ("sprite#inspector");

      std::string new_texture = m_selected_texture;
      if (ImGui::BeginCombo ("Texture", m_selected_texture.c_str(), 0))
      {
        for (const auto& tex : m_texture_list)
        {
          const bool is_selected = (m_selected_texture == tex);
          if (ImGui::Selectable (tex.c_str(), is_selected))
          {
            new_texture = tex;
          }

          if (is_selected)
          {
            ImGui::SetItemDefaultFocus();
          }
        }
        ImGui::EndCombo();
      }
      ImGui::PopID();
    }

  private:
    const std::vector<std::string>& m_texture_list;
    std::string m_selected_texture = "";
  };
} // namespace widget
} // namespace stella