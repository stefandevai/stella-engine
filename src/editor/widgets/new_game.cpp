#include "editor/widgets/new_game.hpp"
#include "editor/widgets/file_dialog.hpp"
#include "editor/actions.hpp"

namespace editor
{
namespace widget
{
  NewGame::NewGame() : Widget ("new-scene-popup") {}

  void NewGame::render (NewGameFunction create_new_game)
  {
    if (!m_open)
    {
      return;
    }

    if (ImGui::Begin("Create a new Game", &m_open))
    {
      static char name[128] = "";
      static char path[512] = "";
      static int dimensions[2] = { 0, 0 };
      const float item_width = ImGui::CalcItemWidth();

      ImGui::Text("Create a new Game");
      ImGui::Separator();
      ImGui::Text("Name:");
      ImGui::InputText("###input-new-game1", name, IM_ARRAYSIZE(name));
      ImGui::Text("Dimensions:");
      ImGui::InputInt2("###input-new-game3", dimensions);
      ImGui::PushItemWidth (item_width - 64.f);
      ImGui::Text("Path:");
      ImGui::InputText("###input-new-game2", path, IM_ARRAYSIZE(path));
      ImGui::PopItemWidth();
      ImGui::SameLine (0.f, 4.f);
      if (ImGui::Button ("...###game-file-dialog-button1", ImVec2 (60.f, 0)))
      {
        ImGuiFileDialog::Instance()->OpenModal ("new-game-file-dialog1", "Select a directory", 0, ".");
      }

      ImGui::Dummy (ImVec2 (0.f, 5.f));

      // Create game if params are not empty
      if(ImGui::Button("Create") && name[0] != 0 && path[0] != 0 && dimensions[0] != 0 && dimensions[1] != 0)
      {
        create_new_game (path, name, dimensions[0], dimensions[1]);
        memset(name, 0, sizeof name);
        memset(path, 0, sizeof path);
        dimensions[0] = 0;
        dimensions[1] = 0;
        m_open = false;
      }

      // File dialog
      if (ImGuiFileDialog::Instance()->FileDialog("new-game-file-dialog1"))
      {
        if (ImGuiFileDialog::Instance()->IsOk == true)
        {
          std::string filepath = ImGuiFileDialog::Instance()->GetFilePathName();
          strcpy(path, filepath.c_str());
        }
        ImGuiFileDialog::Instance()->CloseDialog("new-game-file-dialog1");
      }
    }
    ImGui::End();
  }
} // namespace widget
} // namespace editor

