
#include "editor/widgets/toolbar.h"

namespace stella
{
namespace widget
{
  Toolbar::Toolbar() { m_open = true; }

  Toolbar::~Toolbar() {}

  void Toolbar::render (editor::State& state, editor::Tool& tool)
  {
    ImGui::PushStyleVar (ImGuiStyleVar_FramePadding, ImVec2 (4.f, 4.f));
    // ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.f, 0.5f));
    ImGui::PushStyleVar (ImGuiStyleVar_ItemSpacing, ImVec2 (2.0f, 0.0f));
    ImGui::PushStyleColor (ImGuiCol_Button, ImVec4 (0.0f, 0.0f, 0.f, 0.f));
    ImGui::PushStyleColor (ImGuiCol_ButtonHovered, ImVec4 (0.2f, 0.2f, 0.2f, 1.f));

    if (ImGui::Begin ("toolbar-win", &m_open))
    {
      // Handling inspector button hovering and clicks
      ImGui::PushID ("inspector-button");
      if (state == editor::EDIT && tool == editor::INSPECTOR)
      {
        ImGui::PushStyleColor (ImGuiCol_Button, ImVec4 (0.2f, 0.2f, 0.2f, 1.f));
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
        ImGui::PushStyleColor (ImGuiCol_Button, ImVec4 (0.2f, 0.2f, 0.2f, 1.f));
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
        ImGui::PushStyleColor (ImGuiCol_Button, ImVec4 (0.2f, 0.2f, 0.2f, 1.f));
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

      ImGui::Dummy (ImVec2 (0.f, 2.f));
      ImGui::Separator();
    }
    ImGui::End();
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
    ImGui::PopStyleVar();
    ImGui::PopStyleVar();
  }
} // namespace editor
} // namespace stella
