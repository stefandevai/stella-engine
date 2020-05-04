#include "editor/widgets/inspector.hpp"
#include "editor/widgets/components/components_widgets.hpp"
#include "editor/components/selected.hpp"

// TEMP
#include "stella/components.hpp" // IWYU pragma: export
// TEMP

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
        ImGui::Dummy (ImVec2 (0.f, 10.f));
        ImGui::Separator();
        ImGui::Dummy (ImVec2 (0.f, 10.f));
        if (ImGui::Button("Add Components", ImVec2(100, 27)))
        {
          m_add_components.open();
        }
        m_add_components.render(registry, m_selected_entity);
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
      m_render_component_node<component::AnimationPlayer> (registry, AnimationPlayer());
      m_render_component_node<component::Body2D> (registry, Body2D());
      m_render_component_node<component::Camera> (registry, Camera());
      m_render_component_node<component::CharacterAnimation> (registry, CharacterAnimation());
      m_render_component_node<component::Charcode> (registry, Charcode());
      m_render_component_node<component::Color> (registry, Color());
      m_render_component_node<component::Dimension> (registry, Dimension());
      m_render_component_node<component::LayerT> (registry, Layer());
      m_render_component_node<component::NPC> (registry, NPC());
      m_render_component_node<component::Position> (registry, Position());
      m_render_component_node<component::Script> (registry, Script());
      m_render_component_node<component::Shape> (registry, Shape());
      m_render_component_node<component::SpeechContainer> (registry, SpeechContainer());
      m_render_component_node<component::SpriteGroup> (registry, SpriteGroup());
      m_render_component_node<component::SpriteT> (registry, Sprite());
      m_render_component_node<component::Text> (registry, Text());
      m_render_component_node<component::Tile> (registry, Tile());
      m_render_component_node<component::Timer> (registry, Timer());
      m_render_component_node<component::Typewriter> (registry, Typewriter());
    }
  }
} // namespace widget
} // namespace stella