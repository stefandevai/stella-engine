#include "editor/widgets/edit_mode_main_menu_options.hpp"

namespace editor
{
namespace widget
{
  EditModeMainMenuOptions::EditModeMainMenuOptions () : Widget("EditModeMainMenuOptions") {}

  editor::Action EditModeMainMenuOptions::render()
  {
    editor::Action action = editor::Action::NONE;

    if (ImGui::BeginMainMenuBar())
    {
      if (ImGui::BeginMenu ("File"))
      {
        if (ImGui::MenuItem ("New Game", "CTRL+N"))
        {
          action = editor::Action::NEW_GAME;
        }

        if (ImGui::MenuItem ("Open Game", "CTRL+O"))
        {
          action = editor::Action::OPEN_GAME;
        }

        ImGui::Separator();

        if (ImGui::MenuItem ("Save Game", "CTRL+S"))
        {
          action = editor::Action::SAVE_GAME;
        }

        if (ImGui::MenuItem ("Save Game as...", "CTRL+SHIFT+S"))
        {
          action = editor::Action::SAVE_GAME_AS;
        }

        ImGui::Separator();

        if (ImGui::MenuItem ("Quit", "CTRL+Q"))
        {
          action = editor::Action::QUIT_EDITOR;
        }
        ImGui::EndMenu();
      }
      if (ImGui::BeginMenu ("Scene"))
      {
        if (ImGui::MenuItem ("New Scene", "CTRL+N"))
        {
          action = editor::Action::NEW_SCENE;
        }

        if (ImGui::MenuItem ("Load Scene", "CTRL+S"))
        {
          action = editor::Action::LOAD_SCENE;
        }

        ImGui::Separator();

        if (ImGui::MenuItem ("Save Scene", "CTRL+S"))
        {
          action = editor::Action::SAVE_SCENE;
        }

        if (ImGui::MenuItem ("Save Scene as...", "CTRL+SHIFT+S"))
        {
          action = editor::Action::SAVE_SCENE_AS;
        }

        ImGui::EndMenu();
      }
      if (ImGui::BeginMenu ("View"))
      {
        ImGui::EndMenu();
      }
      ImGui::EndMainMenuBar();
    }

    return action;
  }
} // namespace widget
} // namespace editor


