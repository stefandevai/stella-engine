#include "editor/widgets/scene_editor.hpp"
#include "editor/widgets/file_dialog.hpp"
#include "stella/game2.hpp"
#include "stella/core/scene.hpp"
#include "stella/components/game_object.hpp"
#include "stella/components/name.hpp"
#include <spdlog/spdlog.h>

namespace editor
{
namespace widget
{
  SceneEditor::SceneEditor() : Widget ("Scene Editor") { m_open = true; }

  void SceneEditor::reload() { m_action = SceneEditorAction::RELOAD_EDITOR; }

  void SceneEditor::render (std::shared_ptr<stella::Game>& game)
  {
    if (!m_open)
    {
      return;
    }

    if (ImGui::Begin (m_title_string.c_str(), &m_open))
    {
      std::string selected_scene_name;

      if (ImGui::BeginCombo ("###scene-combo1", game->m_current_scene->get_name().c_str(), 0))
      {
        for (auto& scene : game->m_scenes)
        {
          const bool is_selected = scene == game->m_current_scene;
          if (ImGui::Selectable (scene->get_name().c_str(), is_selected))
          {
            selected_scene_name = scene->get_name();
            m_set_action (SceneEditorAction::SELECT_SCENE);
          }
        }
        ImGui::EndCombo();
      }

      ImGui::Dummy (ImVec2 (0.f, 3.f));

      // Scene script
      static bool script_set       = false;
      static char script_path[512] = "";
      const float item_width       = ImGui::CalcItemWidth();

      if (!script_set && game->m_current_scene != nullptr)
      {
        // Set current script filepath
        strcpy (script_path, game->m_current_scene->m_script_api.get_filepath().c_str());
        script_set = true;
      }

      ImGui::Text ("Script:");
      ImGui::PushItemWidth (item_width - 64.f);
      ImGui::InputText ("###scene-editor-input5", script_path, IM_ARRAYSIZE (script_path));
      ImGui::PopItemWidth();
      ImGui::SameLine (0.f, 4.f);
      if (ImGui::Button ("...###scene-editor-dialog-button1", ImVec2 (30.f, 0)))
      {
        ImGuiFileDialog::Instance()->OpenModal ("scene-editor-file-dialog1", "Script", ".lua", "");
      }
      ImGui::SameLine (0.f, 4.f);
      if (ImGui::Button ("R###scene-editor-dialog-button2", ImVec2 (30.f, 0)))
      {
        game->m_current_scene->m_script_api.load (std::string (script_path));
      }

      // File dialog
      if (ImGuiFileDialog::Instance()->FileDialog ("scene-editor-file-dialog1"))
      {
        if (ImGuiFileDialog::Instance()->IsOk == true)
        {
          std::string filepath = ImGuiFileDialog::Instance()->GetFilePathName();
          strcpy (script_path, filepath.c_str());
          game->m_current_scene->m_script_api.load (filepath);
        }
        ImGuiFileDialog::Instance()->CloseDialog ("scene-editor-file-dialog1");
      }

      ImGui::Dummy (ImVec2 (0.f, 3.f));
      ImGui::Separator();
      ImGui::Dummy (ImVec2 (0.f, 3.f));

      if (game->m_current_scene != nullptr)
      {
        // Scene Objects
        if (ImGui::CollapsingHeader ("Objects###scene-editor-input3"))
        {
          game->m_current_scene->m_registry.view<stella::component::GameObject, stella::component::Name>().each ([] (auto entity, auto& obj, auto& name) {
            ImGui::PushID ((name.name + "#object-list").c_str());
            if (ImGui::MenuItem (name.name.c_str(), "", false)) {}
            ImGui::PopID();
          });

          ImGui::Dummy (ImVec2 (0.f, 1.f));
          if (ImGui::Button ("New Object"))
          {
            std::string object_label = "Object ";
            auto entity              = game->m_current_scene->m_registry.create();
            game->m_current_scene->m_registry.emplace<stella::component::GameObject> (entity);
            game->m_current_scene->m_registry.emplace<stella::component::Name> (entity, object_label.append (std::to_string (m_number_of_objects)));
            m_number_of_objects++;
          }
        }

        ImGui::Dummy (ImVec2 (0.f, 3.f));

        // Scene Systems
        if (ImGui::CollapsingHeader ("Systems###scene-editor-input4"))
        {
          for (auto& system : game->m_current_scene->m_systems)
          {
            ImGui::PushID ((system.second->get_tag() + "#system-list").c_str());
            if (ImGui::MenuItem (system.second->get_tag().c_str(), "", false)) {}
            ImGui::PopID();
          }

          ImGui::Dummy (ImVec2 (0.f, 1.f));
          if (ImGui::Button ("Add System"))
          {
            m_new_system.open();
          }
        }
      }

      auto new_system_tag = m_new_system.render (game->m_current_scene);
      if (!new_system_tag.empty())
      {
        m_set_action (SceneEditorAction::ADD_SYSTEM);
      }

      // if (game->m_current_scene != nullptr && scene_name != game.m_current_scene->get_name())
      //{
      // m_set_action(SceneEditorAction::UPDATE_SCENE);
      //}

      switch (m_action)
      {
        case SceneEditorAction::RELOAD_EDITOR:
        {
          if (game->m_current_scene != nullptr)
          {
            // auto original_scene_name = game->m_current_scene->get_name();
            // original_scene_name.copy(scene_name, original_scene_name.size());
            // scene_name[original_scene_name.size()] = '\0';
            m_action = SceneEditorAction::NONE;
          }
        }
        break;
        case SceneEditorAction::UPDATE_SCENE:
        {
          // game->m_current_scene->set_name(scene_name);
          m_action = SceneEditorAction::NONE;
        }
        break;
        case SceneEditorAction::SELECT_SCENE:
        {
          game->start_scene (selected_scene_name);
          m_action = SceneEditorAction::RELOAD_EDITOR;
        }
        break;
        case SceneEditorAction::ADD_SYSTEM:
        {
          game->m_current_scene->add_system (new_system_tag);
          m_action = SceneEditorAction::NONE;
        }
        break;
        default:
          break;
      }
    }
    ImGui::End();
  }

  void SceneEditor::m_set_action (SceneEditorAction action)
  {
    if (m_action == SceneEditorAction::NONE)
    {
      m_action = action;
    }
  }

  void SceneEditor::open()
  {
    m_action = SceneEditorAction::RELOAD_EDITOR;
    m_open   = true;
  }
} // namespace widget
} // namespace editor
