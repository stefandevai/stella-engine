#include "editor/inspector.h"

namespace stella
{
namespace editor
{
    Inspector::Inspector()
    {

    }
    Inspector::~Inspector() {}

    void Inspector::render()
    {
      if (ImGui::CollapsingHeader("Inspector"))
      {
            ImGui::Dummy(ImVec2(0.f, 3.f));
            ImGui::Text("Inspector!");
            ImGui::Dummy(ImVec2(0.f, 12.f));
      }
    }
}
}