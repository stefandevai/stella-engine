#include "editor/layer_editor.h"

namespace stella
{
namespace editor
{
    LayerEditor::LayerEditor()
    {

    }

    LayerEditor::~LayerEditor() {}

    void LayerEditor::render()
    { 
      if (ImGui::CollapsingHeader("Map", ImGuiTreeNodeFlags_DefaultOpen))
      {
        ImGui::Dummy(ImVec2(0.f, 3.f));

        static char str1[128] = "Test map";
        ImGui::InputText("   Name", str1, IM_ARRAYSIZE(str1));
        ImGui::Dummy(ImVec2(0.f, 3.f));
        static char str2[128] = "./scripts/map.xml";
        ImGui::InputTextWithHint("   Path", "Map file location.", str2, IM_ARRAYSIZE(str2));
        ImGui::Dummy(ImVec2(0.f, 3.f));
        int map_size_test[2] = {60, 30};
        ImGui::PushID("map-slider");
        ImGui::SliderInt2("   Size", map_size_test, 1, 1000);
        ImGui::PopID();
        ImGui::Dummy(ImVec2(0.f, 3.f));

        // Layers
        if (ImGui::TreeNodeEx("Layers", ImGuiTreeNodeFlags_DefaultOpen))
        {
        ImGui::BeginGroup();
        ImGui::Selectable("Layer1", true);
        ImGui::Selectable("Layer2", false);
        ImGui::Selectable("Layer3", false);
        ImGui::EndGroup();
        ImGui::TreePop();
        }
        ImGui::Dummy(ImVec2(0.0f, 12.0f));
      }
    }
}
}