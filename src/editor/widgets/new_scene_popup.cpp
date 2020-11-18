#include "editor/widgets/new_scene_popup.hpp"
#include "stella/game2.hpp"

#include <iostream>

namespace stella
{
namespace widget
{
  NewScenePopup::NewScenePopup(stella::Game& game) : Widget ("new-scene-popup"), m_game(game) {}

  bool NewScenePopup::render ()
  {
    bool created_scene = false;

    if (ImGui::BeginPopup("new_scene_popup"))
    {
      static char scene_name[128] = "";
      static char scene_script_path[512] = "";

      ImGui::Text("Create a new Scene");
      ImGui::InputText("Name", scene_name, IM_ARRAYSIZE(scene_name));
      ImGui::InputText("Script Path", scene_script_path, IM_ARRAYSIZE(scene_script_path));

      if(ImGui::Button("Create"))
      {
        auto scene_name_str = std::string(scene_name);
        m_game.create_scene(scene_name_str, std::string(scene_script_path));
        m_game.load_scene(scene_name_str);
        created_scene = true;
        memset(scene_name, 0, sizeof scene_name);
        memset(scene_script_path, 0, sizeof scene_script_path);
        ImGui::CloseCurrentPopup();
      }

      ImGui::EndPopup();
    }

    return created_scene;
  }

  void NewScenePopup::open()
  {
    ImGui::OpenPopup("new_scene_popup");
  }
} // namespace widget
} // namespace stella

