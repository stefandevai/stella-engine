
#include "editor/widgets/toolbar.hpp"
#include "editor/components/selected.hpp"
#include "stella/components/position.hpp"
#include "stella/components/dimension.hpp"
#include "stella/components/sprite.hpp"

#include <iostream>

namespace stella
{
namespace widget
{
  Toolbar::Toolbar()
  : Widget ("Toolbar")
  { m_open = true; }

  Toolbar::~Toolbar() {}

  void
  Toolbar::render (entt::registry& registry, editor::State& state, editor::Tool& tool, const float width, std::function<void()> draw_menu_bar)
  {
    ImGui::SetNextWindowSize (ImVec2 (width, 60), ImGuiCond_Always);
    ImGui::SetNextWindowPos (ImVec2 (0, 0), ImGuiCond_Always);

    // if (ImGui::Begin ("toolbar-win", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove |
    // ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize))
    if (ImGui::Begin ("toolbar-win",
                      nullptr,
                      ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse |
                          ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar |
                          ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus))
    {
      draw_menu_bar();

      ImGui::PushStyleVar (ImGuiStyleVar_FramePadding, ImVec2 (4.f, 2.f));
      ImGui::PushStyleVar (ImGuiStyleVar_ItemSpacing, ImVec2 (1.0f, 0.0f));
      ImGui::PushStyleColor (ImGuiCol_Text, m_button_color);
      ImGui::PushStyleColor (ImGuiCol_Button, ImVec4 (0.0f, 0.0f, 0.0f, 0.0f));
      ImGui::PushStyleColor (ImGuiCol_ButtonHovered, m_button_hover_color);
      ImGui::PushStyleColor (ImGuiCol_ButtonActive, ImVec4 (0.3f, 0.3f, 0.3f, 1.0f));
      m_size = ImGui::GetWindowSize();

      m_render_inspector_button (state, tool);
      ImGui::SameLine();
      m_render_tilepen_button (state, tool);
      ImGui::SameLine();
      m_render_play_button (state, tool);
      ImGui::SameLine();
      m_render_plus_button (registry, width);

      ImGui::PopStyleColor();
      ImGui::PopStyleColor();
      ImGui::PopStyleColor();
      ImGui::PopStyleColor();
      ImGui::PopStyleVar();
      ImGui::PopStyleVar();
    }
    ImGui::End();
  }

  void Toolbar::m_render_inspector_button (editor::State& state, editor::Tool& tool)
  {
    ImGui::PushID ("inspector-button");
    if (state == editor::EDIT && tool == editor::INSPECTOR)
      {
        ImGui::PushStyleColor (ImGuiCol_Text, m_button_selected_color);
      }

      const bool clicked_inspector = ImGui::Button (ICON_FA_MOUSE_POINTER);
      if (tool != editor::INSPECTOR && clicked_inspector)
      {
        if (state != editor::EDIT)
        {
          state = editor::EDIT;
        }
        tool = editor::INSPECTOR;
      }
      else if (state == editor::EDIT && tool == editor::INSPECTOR)
      {
        ImGui::PopStyleColor();
      }
      ImGui::PopID();
  }

  void Toolbar::m_render_tilepen_button (editor::State& state, editor::Tool& tool)
  {
    ImGui::PushID ("tile-pen-button");
      if (state == editor::EDIT && tool == editor::TILE_PEN)
      {
        ImGui::PushStyleColor (ImGuiCol_Text, m_button_selected_color);
      }
      const bool clicked_tile_pen = ImGui::Button (ICON_FA_EDIT);
      if (tool != editor::TILE_PEN && clicked_tile_pen)
      {
        if (state != editor::EDIT)
        {
          state = editor::EDIT;
        }
        tool = editor::TILE_PEN;
      }
      else if (state == editor::EDIT && tool == editor::TILE_PEN)
      {
        ImGui::PopStyleColor();
      }
      ImGui::PopID();
  }

  void Toolbar::m_render_play_button (editor::State& state, editor::Tool& tool)
  {
    ImGui::PushID ("play-button");
      if (state == editor::PLAY)
      {
        ImGui::PushStyleColor (ImGuiCol_Text, m_button_selected_color);
      }
      const bool clicked_play = ImGui::Button (ICON_FA_PLAY);
      if (state != editor::PLAY && clicked_play)
      {
        state = editor::PLAY;
      }
      else if (state == editor::PLAY)
      {
        ImGui::PopStyleColor();
      }
      ImGui::PopID();
  }

  void Toolbar::m_render_plus_button (entt::registry& registry, const float width)
  {
    ImGui::PushID ("plus-button");
      // const bool clicked_plus = ;
      if (ImGui::Button (ICON_FA_PLUS))
      {
        // registry.create();
        ImGui::OpenPopup("Create new Entity"); 
      }
      m_add_new_entity(registry, width);
      ImGui::PopID();
  }

  void Toolbar::m_add_new_entity(entt::registry& registry, const float width)
  {
    ImGui::SetNextWindowPos(ImVec2(width/2.f - width*0.7f/2.f, 30.f), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(width*0.7f, 500.f), ImGuiCond_Always);
    if (ImGui::BeginPopup("Create new Entity"))
    {
      ImGui::Text("Create a new Entity");
      if (ImGui::Button("Create"))
      {
        auto entity = registry.create();
        registry.emplace<component::Position>(entity, 500.f, 500.f, 0.0f);
        registry.emplace<component::Dimension>(entity, 32.f, 64.f);
        auto& sprite = registry.emplace<component::SpriteT>(entity, "nikte");
        sprite.hframes = 9;
        sprite.vframes = 6;
        sprite.layer = "collision";
        registry.emplace<component::Selected>(entity);
      }
      ImGui::EndPopup();
    }
  }

} // namespace widget
} // namespace stella
