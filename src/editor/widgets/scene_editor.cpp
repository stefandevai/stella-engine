#include "editor/widgets/scene_editor.hpp"
#include "editor/widgets/file_dialog.hpp"
#include "stella/game2.hpp"
#include "stella/core/scene.hpp"
#include "stella/components/game_object.hpp"
#include "stella/components/name.hpp"

// TEMP
#include <spdlog/spdlog.h>
// TEMP

namespace editor
{
namespace widget
{
  SceneEditor::SceneEditor() : Widget ("Scene Editor") { m_open = true; }

  void SceneEditor::reload()
  {
    m_action = SceneEditorAction::RELOAD_EDITOR;
  }

  void SceneEditor::render (std::shared_ptr<stella::Game>& game)
  {
    if (!m_open)
    {
      return;
    }

    if (ImGui::Begin (m_title_string.c_str(), &m_open))
    {
      std::string selected_scene_name;

      // Scene name
      //ImGui::Dummy (ImVec2 (0.f, 3.f));
      //ImGui::Text ("Name:");
      //ImGui::PushID ("scene-editor-input#1");
      //ImGui::InputTextWithHint ("", "", scene_name, IM_ARRAYSIZE (scene_name));
      //ImGui::PopID();
      //ImGui::Dummy (ImVec2 (0.f, 3.f));

      if (ImGui::BeginCombo("###scene-combo1", game->m_current_scene->get_name().c_str(), 0))
      {
        for (auto& scene : game->m_scenes)
        {
          const bool is_selected = scene == game->m_current_scene;
          if (ImGui::Selectable(scene->get_name().c_str(), is_selected))
          {
            selected_scene_name = scene->get_name();
            m_set_action(SceneEditorAction::SELECT_SCENE);
          }
        }
        ImGui::EndCombo();
      }
      ImGui::Separator();

      if (game->m_current_scene != nullptr)
      {
        if (ImGui::CollapsingHeader("Objects###scene-editor-input3"))
        {
          game->m_current_scene->m_registry.view<stella::component::GameObject, stella::component::Name>().each ([] (auto entity, auto& obj, auto& name) {
            ImGui::PushID ((name.name + "#object-list").c_str());
            if (ImGui::MenuItem (name.name.c_str(), "", false)) {}
            ImGui::PopID();
          });

          if (ImGui::Button ("New Object"))
          {
            std::string object_label = "Object ";
            auto entity = game->m_current_scene->m_registry.create();
            game->m_current_scene->m_registry.emplace<stella::component::GameObject> (entity);
            game->m_current_scene->m_registry.emplace<stella::component::Name> (entity, object_label.append(std::to_string(m_number_of_objects)));
            m_number_of_objects++;
          }
        }
      }

      //if (game->m_current_scene != nullptr && scene_name != game.m_current_scene->get_name())
      //{
        //m_set_action(SceneEditorAction::UPDATE_SCENE);
      //}

      switch (m_action)
      {
        case SceneEditorAction::RELOAD_EDITOR:
          {
            if (game->m_current_scene != nullptr)
            {
              //auto original_scene_name = game->m_current_scene->get_name();
              //original_scene_name.copy(scene_name, original_scene_name.size());
              //scene_name[original_scene_name.size()] = '\0';
              m_action = SceneEditorAction::NONE;
            }
          }
          break;
        case SceneEditorAction::UPDATE_SCENE:
          {
            //game->m_current_scene->set_name(scene_name);
            m_action = SceneEditorAction::NONE;
          }
          break;
        case SceneEditorAction::SELECT_SCENE:
          {
            game->start_scene(selected_scene_name);
            m_action = SceneEditorAction::RELOAD_EDITOR;
          }
          break;
        default:
          break;
      }
    }
    ImGui::End();
  }
  
  void SceneEditor::m_set_action(SceneEditorAction action)
  {
    if (m_action == SceneEditorAction::NONE)
    {
      m_action = action;
    }
  }

  void SceneEditor::open ()
  {
    m_action = SceneEditorAction::RELOAD_EDITOR;
    m_open = true;
  }
} // namespace widget
} // namespace editor


