#include "editor/widgets/load_scene_popup.hpp"
#include "editor/widgets/file_dialog.hpp"
#include "stella/game2.hpp"

#include <iostream>

namespace editor
{
namespace widget
{
  LoadScenePopup::LoadScenePopup(stella::Game& game) : Widget ("load-scene-popup"), m_game(game) {}

  void LoadScenePopup::open ()
  {
    ImGuiFileDialog::Instance()->OpenDialog ("load-scene-file-dialog1", "New Scene", ".json", ".");
  }

  bool LoadScenePopup::render ()
  {
    bool loaded_scene = false;

    if (ImGuiFileDialog::Instance()->FileDialog("load-scene-file-dialog1"))
    {
      if (ImGuiFileDialog::Instance()->IsOk == true)
      {
        std::string filepath = ImGuiFileDialog::Instance()->GetFilepathName();
        m_game.load_scene(filepath);
        m_game.start_current_scene();
        loaded_scene = true;
      }
      ImGuiFileDialog::Instance()->CloseDialog("load-scene-file-dialog1");
    }

    return loaded_scene;
  }
} // namespace widget
} // namespace editor

