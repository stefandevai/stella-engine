#include "editor/widgets/load_scene_popup.hpp"
#include "editor/widgets/file_dialog.hpp"
#include "stella/game2.hpp"

namespace editor
{
namespace widget
{
  LoadScenePopup::LoadScenePopup(stella::Game& game) : Widget ("load-scene-popup"), m_game(game) {}

  void LoadScenePopup::open ()
  {
    ImGuiFileDialog::Instance()->OpenModal ("load-scene-file-dialog1", "New Scene", ".json", ".");
  }

  void LoadScenePopup::render ()
  {
    if (ImGuiFileDialog::Instance()->FileDialog("load-scene-file-dialog1"))
    {
      if (ImGuiFileDialog::Instance()->IsOk == true)
      {
        std::string filepath = ImGuiFileDialog::Instance()->GetFilePathName();
        m_game.load_scene(filepath);
        m_game.start_current_scene();
      }
      ImGuiFileDialog::Instance()->CloseDialog("load-scene-file-dialog1");
    }
  }
} // namespace widget
} // namespace editor

