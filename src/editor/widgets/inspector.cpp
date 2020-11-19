#include "editor/widgets/inspector.hpp"
#include "editor/components/selected.hpp"
#include "editor/widgets/entity_components.hpp"
#include "editor/widgets/components/components_widgets.hpp" // IWYU pragma: export

// TEMP
#include "stella/components.hpp" // IWYU pragma: export
// TEMP

namespace editor
{
namespace widget
{
  Inspector::Inspector() : Widget ("Inspector") { m_open = true; }

  // void Inspector::render (entt::registry& registry, const std::vector<std::string&>& texture_list)
  void Inspector::render (entt::registry& registry, const std::vector<std::string>& texture_list)
  {
    if (!m_open)
    {
      return;
    }

    std::vector<entt::entity> selected_entities;
    registry.view<component::Selected>().each ([&selected_entities] (auto entity, auto& sel) {
      selected_entities.push_back (entity);
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

    if (ImGui::Begin (m_title_string.c_str(), &m_open))
    {
      ImGui::Dummy (ImVec2 (0.f, 3.f));
      if (m_selected_entity != entt::null && registry.valid (m_selected_entity))
      {
        m_render_component_nodes (registry, texture_list);
        ImGui::Dummy (ImVec2 (0.f, 10.f));
        ImGui::Separator();
        ImGui::Dummy (ImVec2 (0.f, 10.f));
        if (ImGui::Button ("Add Components", ImVec2 (100, 27)))
        {
          m_add_components.open();
        }
        m_add_components.render (registry, m_selected_entity);
      }
      else
      {
        ImGui::Text ("Use the Inspector Tool %s to select an entity.", u8"\uf245");
      }
      ImGui::Dummy (ImVec2 (0.f, 12.f));
    }
    ImGui::End();
  }

  void Inspector::m_render_component_nodes (entt::registry& registry, const std::vector<std::string>& texture_list)
  {
    if (ImGui::CollapsingHeader ("Components", ImGuiTreeNodeFlags_DefaultOpen))
    {
      m_components.set_entity (m_selected_entity);
      m_components.render_component_node<stella::component::AnimationPlayer> (registry, AnimationPlayer());
      m_components.render_component_node<stella::component::Body2D> (registry, Body2D());
      m_components.render_component_node<stella::component::Camera> (registry, Camera());
      m_components.render_component_node<stella::component::CharacterAnimation> (registry, CharacterAnimation());
      m_components.render_component_node<stella::component::Charcode> (registry, Charcode());
      m_components.render_component_node<stella::component::Color> (registry, Color());
      m_components.render_component_node<stella::component::Dimension> (registry, Dimension());

      // TODO: Understand why the functor is always passed by value in this case
      m_group_widget.set_texture_list (texture_list);
      if (registry.has<stella::component::Group> (m_selected_entity))
      {
        auto& group = registry.get<stella::component::Group> (m_selected_entity);
        if (ImGui::TreeNode (group.name.c_str()))
        {
          m_group_widget.render (registry, m_selected_entity);
          ImGui::Dummy (ImVec2 (0, 2.0));
          ImGui::TreePop();
        }
      }

      // m_components.render_component_node<stella::component::Group> (registry, m_group_widget);
      m_components.render_component_node<stella::component::LayerT> (registry, Layer());
      m_components.render_component_node<stella::component::NPC> (registry, NPC());
      m_components.render_component_node<stella::component::Position> (registry, Position());
      m_components.render_component_node<stella::component::Script> (registry, Script());
      m_components.render_component_node<stella::component::Shape> (registry, Shape());
      m_components.render_component_node<stella::component::SpeechContainer> (registry, SpeechContainer());
      m_components.render_component_node<stella::component::SpriteT> (registry, Sprite (texture_list));
      m_components.render_component_node<stella::component::Text> (registry, Text());
      m_components.render_component_node<stella::component::Tile> (registry, Tile());
      m_components.render_component_node<stella::component::Timer> (registry, Timer());
      m_components.render_component_node<stella::component::Typewriter> (registry, Typewriter());
      m_components.render_component_node<stella::component::Vertical> (registry, Vertical());
    }
  }
} // namespace widget
} // namespace editor
