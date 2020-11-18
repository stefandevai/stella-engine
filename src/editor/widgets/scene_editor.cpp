#include "editor/widgets/scene_editor.hpp"
#include "stella/core/scene.hpp"
#include "stella/components/game_object.hpp"
#include "stella/components/name.hpp"

// TEMP
#include <iostream>
// TEMP

namespace stella
{
namespace widget
{
  SceneEditor::SceneEditor() : Widget ("Scene Editor") { m_open = true; }

  void SceneEditor::reload()
  {
    m_should_reload = true;
  }

  void SceneEditor::render (std::shared_ptr<core::Scene>& scene)
  {
    if (ImGui::Begin (m_title_string.c_str(), &m_open))
    {
      static char scene_name[128] = "";
      static char scene_script_path[512] = "";
      const float item_width = ImGui::CalcItemWidth();

      if (m_should_reload)
      {
        if (scene != nullptr)
        {
          auto original_scene_name = scene->get_name();
          auto original_scene_script_path = scene->get_script_path();
          original_scene_name.copy(scene_name, original_scene_name.size());
          original_scene_script_path.copy(scene_script_path, original_scene_script_path.size());
          scene_name[original_scene_name.size()] = '\0';
          scene_script_path[original_scene_script_path.size()] = '\0';
        }
        m_should_reload = false;
      }

      // Scene name
      ImGui::Dummy (ImVec2 (0.f, 3.f));
      ImGui::Text ("Name:");
      ImGui::PushID ("scene-editor-input#1");
      ImGui::InputTextWithHint ("", "", scene_name, IM_ARRAYSIZE (scene_name));
      ImGui::PopID();
      ImGui::Dummy (ImVec2 (0.f, 3.f));

      // Scene script path
      ImGui::Text ("Script path:");
      ImGui::PushItemWidth (item_width - 64.f);
      ImGui::PushID ("scene-editor-input#2");
      ImGui::InputTextWithHint ("", "", scene_script_path, IM_ARRAYSIZE (scene_script_path));
      ImGui::PopID();
      ImGui::PopItemWidth();
      ImGui::SameLine (0.f, 4.f);
      ImGui::PushID ("scene-editor-input#3");
      if (ImGui::Button ("...", ImVec2 (60.f, 0)))
      {
        //load();
      }
      ImGui::PopID();
      ImGui::Dummy (ImVec2 (0.f, 3.f));
      ImGui::Separator();

      if (scene == nullptr)
      {
        ImGui::Text("Please add a new Scene.");
      }
      else
      {
        if (ImGui::CollapsingHeader("Objects###scene-editor-input3"))
        {
          scene->m_registry.view<component::GameObject, component::Name>().each ([] (auto entity, auto& obj, auto& name) {
            ImGui::PushID ((name.name + "#object-list").c_str());
            if (ImGui::MenuItem (name.name.c_str(), "", false)) {}
            ImGui::PopID();
          });

          if (ImGui::Button ("New Object"))
          {
            std::string object_label = "Object ";
            auto entity = scene->m_registry.create();
            scene->m_registry.emplace<component::GameObject> (entity);
            scene->m_registry.emplace<component::Name> (entity, object_label.append(std::to_string(m_number_of_objects)));
            m_number_of_objects++;
          }
        }
      }
    }
    ImGui::End();
  }

  void SceneEditor::open ()
  {
    m_should_reload = true;
    m_open = true;
  }
} // namespace widget
} // namespace stella


