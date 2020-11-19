#include "editor/widgets/block_editor.hpp"
#include <string>

namespace editor
{
namespace widget
{
  BlockEditor::BlockEditor() : Widget ("Block Editor") { m_reset_tiles(); }

  void BlockEditor::render()
  {
    if (m_open)
    {
      ImGui::Begin ("Block Editor", &m_open);
      ImGui::InputInt ("width", &m_w);
      ImGui::InputInt ("height", &m_h);
      ImGui::InputInt ("depth", &m_d);

      ImGui::Dummy (ImVec2 (0.f, 5.f));

      for (int j = 0; j < m_d - 1; ++j)
      {
        for (int i = 0; i < m_w; ++i)
        {
          ImGui::SetNextItemWidth (32);
          std::string block_id = "depth" + std::to_string (i + j * m_w);
          ImGui::PushID (block_id.c_str());
          // ImGui::Text(block_id.c_str());
          ImGui::DragInt ("", &m_tiles[i][j]);
          ImGui::PopID();
          ImGui::SameLine();
        }
        ImGui::NewLine();
      }

      for (int j = 0; j < m_h; ++j)
      {
        for (int i = 0; i < m_w; ++i)
        {
          ImGui::SetNextItemWidth (32);
          std::string block_id = "block" + std::to_string (i + (m_d - 1) * m_w + j * m_w);
          ImGui::PushID (block_id.c_str());
          // ImGui::Text(block_id.c_str());
          ImGui::DragInt ("", &m_tiles[i][j + m_d - 1]);
          ImGui::PopID();
          ImGui::SameLine();
        }
        ImGui::NewLine();
      }

      ImGui::Dummy (ImVec2 (0.f, 5.f));

      if (ImGui::Button ("Create Block")) {}
      ImGui::End();
    }
  }

  void BlockEditor::m_reset_tiles()
  {
    for (int i = 0; i < 256; ++i)
    {
      for (int j = 0; j < 256; ++j)
      {
        m_tiles[i][j] = 0;
      }
    }
  }

} // namespace widget
} // namespace editor
