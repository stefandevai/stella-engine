
#include "editor/widgets/toolbar.h"

#include <iostream>

namespace stella
{
namespace widget
{
  Toolbar::Toolbar() : Widget("Toolbar") { m_open = true; }

  Toolbar::~Toolbar() {}

  void Toolbar::render (editor::State& state, editor::Tool& tool, const float width, std::function<void()> draw_menu_bar)
  {
    ImGui::SetNextWindowSize (ImVec2 (width, 60), ImGuiCond_Always);
    ImGui::SetNextWindowPos (ImVec2 (0, 0), ImGuiCond_Always);

    // if (ImGui::Begin ("toolbar-win", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize))
    if (ImGui::Begin ("toolbar-win", nullptr, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse |
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
      // Handling inspector button hovering and clicks
      ImGui::PushID ("inspector-button");

      ImVec4 col_test{1.f, 1.f, 1.f, 1.0};
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
      ImGui::SameLine();

      // Handling tile pen button hovering and clicks
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
      ImGui::SameLine();

      // Handling play button hovering and clicks
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

      // ImGui::Dummy (ImVec2 (0.f, 2.f));
      // ImGui::Separator();
      ImGui::PopStyleColor();
      ImGui::PopStyleColor();
      ImGui::PopStyleColor();
      ImGui::PopStyleColor();
      ImGui::PopStyleVar();
      ImGui::PopStyleVar();
    }
    ImGui::End();
  }
} // namespace editor
} // namespace stella
