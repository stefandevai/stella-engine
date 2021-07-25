#include "editor/widgets/new_scene_popup.hpp"
#include "editor/widgets/file_dialog.hpp"
#include "stella/game.hpp"

namespace editor
{
namespace widget
{
  NewScenePopup::NewScenePopup (std::shared_ptr<stella::Game>& game) : Widget ("new-scene-popup"), m_game (game) {}

  void NewScenePopup::render()
  {
    if (!m_open)
    {
      return;
    }

    if (ImGui::Begin ("Create a new Scene", &m_open))
    {
      static char scene_name[128]        = "";
      static char scene_script_path[512] = "";
      const float item_width             = ImGui::CalcItemWidth();

      ImGui::Text ("Create a new Scene");
      ImGui::Separator();
      ImGui::Text ("Name:");
      ImGui::InputText ("###input-new-scene1", scene_name, IM_ARRAYSIZE (scene_name));
      ImGui::PushItemWidth (item_width - 64.f);
      ImGui::Text ("Path:");
      ImGui::InputText ("###input-new-scene2", scene_script_path, IM_ARRAYSIZE (scene_script_path));
      ImGui::PopItemWidth();
      ImGui::SameLine (0.f, 4.f);
      if (ImGui::Button ("...###scene-file-dialog-button1", ImVec2 (60.f, 0)))
      {
        ImGuiFileDialog::Instance()->OpenModal ("new-scene-file-dialog1", "New Scene", ".json", "");
      }

      ImGui::Dummy (ImVec2 (0.f, 5.f));

      // Create a scene if scene_name and scene_script_path are not empty
      if (ImGui::Button ("Create") && scene_name[0] != 0 && scene_script_path[0] != 0)
      {
        auto scene_name_str = std::string (scene_name);
        m_game->create_scene (scene_name_str, std::string (scene_script_path));
        m_game->start_scene (scene_name_str);
        memset (scene_name, 0, sizeof scene_name);
        memset (scene_script_path, 0, sizeof scene_script_path);
        m_open = false;
      }

      // File dialog
      if (ImGuiFileDialog::Instance()->FileDialog ("new-scene-file-dialog1"))
      {
        if (ImGuiFileDialog::Instance()->IsOk == true)
        {
          std::string filepath = ImGuiFileDialog::Instance()->GetFilePathName();
          strcpy (scene_script_path, filepath.c_str());
        }
        ImGuiFileDialog::Instance()->CloseDialog ("new-scene-file-dialog1");
      }
    }
    ImGui::End();
  }
} // namespace widget
} // namespace editor
