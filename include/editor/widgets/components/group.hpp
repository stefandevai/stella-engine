#pragma once

/* #include "stella/components/group.hpp" */
#include "imgui.h"
#include "editor/widgets/entity_components.hpp"
#include "editor/widgets/add_components.hpp"

#include "animation_player.hpp"
#include "body2d.hpp"
#include "camera.hpp"
#include "character_animation.hpp"
#include "charcode.hpp"
#include "color.hpp"
#include "dimension.hpp"
#include "layer.hpp"
#include "npc.hpp"
#include "position.hpp"
#include "script.hpp"
#include "shape.hpp"
#include "speech_container.hpp"
/* #include "group.hpp" */
#include "sprite.hpp"
#include "text.hpp"
#include "tile.hpp"
#include "timer.hpp"
#include "typewriter.hpp"
#include "vertical.hpp"

// TEMP
#include <iostream>
// TEMP

namespace editor
{
namespace widget
{
  struct Group
  {
    Group() {}

    void set_texture_list (const std::vector<std::string>& texture_list) { m_texture_list = texture_list; }

    void render (entt::registry& registry, const entt::entity entity)
    {
      auto& group             = registry.get<stella::component::Group> (entity);
      entt::entity new_entity = entt::null;

      ImGui::PushID ("group#widget");

      int entt_counter = 1;
      for (auto child : group.children)
      {
        std::string entt_name = "Entity " + std::to_string (entt_counter);
        if (ImGui::CollapsingHeader (entt_name.c_str(), 0))
        {
          EntityComponents components{child};
          components.render_component_node<stella::component::AnimationPlayer> (registry, AnimationPlayer());
          components.render_component_node<stella::component::Body2D> (registry, Body2D());
          components.render_component_node<stella::component::Camera> (registry, Camera());
          components.render_component_node<stella::component::CharacterAnimation> (registry, CharacterAnimation());
          components.render_component_node<stella::component::Charcode> (registry, Charcode());
          components.render_component_node<stella::component::Color> (registry, Color());
          components.render_component_node<stella::component::Dimension> (registry, Dimension());
          // components.render_component_node<stella::component::Group> (registry, Group(m_texture_list));
          components.render_component_node<stella::component::LayerT> (registry, Layer());
          components.render_component_node<stella::component::NPC> (registry, NPC());
          components.render_component_node<stella::component::Position> (registry, Position());
          components.render_component_node<stella::component::Script> (registry, Script());
          components.render_component_node<stella::component::Shape> (registry, Shape());
          components.render_component_node<stella::component::SpeechContainer> (registry, SpeechContainer());
          components.render_component_node<stella::component::SpriteT> (registry, Sprite (m_texture_list));
          components.render_component_node<stella::component::Text> (registry, Text());
          components.render_component_node<stella::component::Tile> (registry, Tile());
          components.render_component_node<stella::component::Timer> (registry, Timer());
          components.render_component_node<stella::component::Typewriter> (registry, Typewriter());
          components.render_component_node<stella::component::Vertical> (registry, Vertical());

          if (ImGui::Button ("Add Components#2", ImVec2 (100, 27)))
          {
            m_add_components.open();
            m_selected_entity = child;
          }
        }
        ++entt_counter;
      }

      m_add_components.render (registry, m_selected_entity);

      if (ImGui::Button ("Add Entity"))
      {
        new_entity = registry.create();
      }
      ImGui::PopID();

      if (new_entity != entt::null)
      {
        registry.patch<stella::component::Group> (entity, [&new_entity] (auto& group) { group.children.push_back (new_entity); });
      }
    }

  private:
    std::vector<std::string> m_texture_list{};
    AddComponents m_add_components{};
    entt::entity m_selected_entity = entt::null;
  };
} // namespace widget
} // namespace editor
