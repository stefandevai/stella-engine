#include "editor/inspector.h"
#include "stella/components/position_component.h"

namespace stella
{
namespace editor
{
    Inspector::Inspector()
    {

    }
    Inspector::~Inspector() {}

    void Inspector::render(entt::registry& registry)
    {
      if (ImGui::CollapsingHeader("Inspector"))
      {
            ImGui::Dummy(ImVec2(0.f, 3.f));
            if (m_selected_entity != entt::null && registry.valid(m_selected_entity))
            {
              if (registry.has<components::PositionComponent>(m_selected_entity))
              {
                auto& pos = registry.get<components::PositionComponent>(m_selected_entity);
                float pos_input[3] = {pos.x, pos.y, pos.z};
                ImGui::InputFloat3("Position", pos_input, "%.2f");
                pos.x = pos_input[0];
                pos.y = pos_input[1];
                pos.z = pos_input[2];
              }
            }
            else
            {
              ImGui::Text("Use the Inspector Tool %s to select an entity.", u8"\uf245");
            }
            ImGui::Dummy(ImVec2(0.f, 12.f));
      }
    }
}
}