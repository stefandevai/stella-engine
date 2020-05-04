#include "editor/widgets/inspector.hpp"
#include "stella/components.hpp" // IWYU pragma: export
#include "editor/components/selected.hpp"

namespace stella
{
namespace widget
{
  Inspector::Inspector() : Widget ("Inspector") { m_open = true; }

  void Inspector::render (entt::registry& registry)
  {
    std::vector<entt::entity> selected_entities;
    registry.view<component::Selected> ().each ([&selected_entities] (auto entity, auto& sel)
    {
      selected_entities.push_back(entity);
      if (selected_entities.size() > 1)
      {
        return;
      }
    });

    // If only one entity is selected
    if (selected_entities.size() == 1)
    {
      m_selected_entity = selected_entities.front();
    }
    // If none or multiple entities are selected
    else
    {
      m_selected_entity = entt::null;
    }

    if (ImGui::Begin (m_name.c_str(), &m_open))
    {
      ImGui::Dummy (ImVec2 (0.f, 3.f));
      if (m_selected_entity != entt::null && registry.valid (m_selected_entity))
      {
        m_render_component_nodes (registry);
      }
      else
      {
        ImGui::Text ("Use the Inspector Tool %s to select an entity.", u8"\uf245");
      }
      ImGui::Dummy (ImVec2 (0.f, 12.f));
    }
    ImGui::End();
  }

  void Inspector::m_render_component_nodes (entt::registry& registry)
  {
    if (ImGui::CollapsingHeader ("Components", ImGuiTreeNodeFlags_DefaultOpen))
    {
      m_render_component_node<component::Position> ("Position", registry, [] (component::Position& pos) {
        float pos_input[3] = {0.0f, 0.0f, 0.0f};
        pos_input[0]       = pos.x;
        pos_input[1]       = pos.y;
        pos_input[2]       = pos.z;
        ImGui::PushID ("position#inspector");
        ImGui::DragFloat ("x", &pos_input[0], 1.0f, 0.0f, 0.0f, "%.3f");
        ImGui::DragFloat ("y", &pos_input[1], 1.0f, 0.0f, 0.0f, "%.3f");
        ImGui::DragFloat ("z", &pos_input[2], 1.0f, 0.0f, 0.0f, "%.3f");
        ImGui::PopID();
        pos.x = pos_input[0];
        pos.y = pos_input[1];
        pos.z = pos_input[2];
      });

      m_render_component_node<component::Dimension> ("Dimension", registry, [] (component::Dimension& dim) {
        float dim_input[2] = {0.0f, 0.0f};
        dim_input[0]       = dim.w;
        dim_input[1]       = dim.h;
        ImGui::PushID ("dimension#inspector");
        ImGui::DragFloat ("width", &dim_input[0], 1.0f, 0.0f, 0.0f, "%.3f");
        ImGui::DragFloat ("height", &dim_input[1], 1.0f, 0.0f, 0.0f, "%.3f");
        ImGui::PopID();
        dim.w = dim_input[0];
        dim.h = dim_input[1];
      });

      m_render_component_node<component::Tile> ("Tile", registry, [] (component::Tile& tile) {
        int layer_id    = static_cast<int> (tile.layer_id);
        bool collidable = tile.collidable;
        ImGui::PushID ("tile#inspector");
        ImGui::InputInt ("layer_id", &layer_id);
        ImGui::Checkbox ("collidable", &collidable);
        ImGui::PopID();
        tile.layer_id   = static_cast<unsigned> (layer_id);
        tile.collidable = collidable;
      });

      m_render_component_node<component::Body2D> ("Tile", registry, [] (component::Body2D& body) {
        float movement_speed = body.movement_speed;
        bool is_static       = body.Body->IsStatic;
        ImGui::PushID ("body#inspector");
        ImGui::DragFloat ("Movement speed", &movement_speed, 1.0f, 0.0f, 0.0f);
        ImGui::Checkbox ("Static", &is_static);
        ImGui::PopID();
        body.movement_speed = movement_speed;
        body.Body->IsStatic = is_static;
      });
    }
  }
} // namespace widget
} // namespace stella