#include <string>
#include <iostream>
#include <memory>

#include "editor/tileset_editor.h"

namespace stella
{
namespace editor
{
    TilesetEditor::TilesetEditor(const std::string& path)
        : m_path(path), m_texture(path)
      {
        m_texture_w = m_texture.GetWidth();
        m_texture_h = m_texture.GetHeight();
      }

      TilesetEditor::~TilesetEditor() {}

      int TilesetEditor::get_selected_tile_id()
      {
          return m_selected_tile_id;
      }
      
      void TilesetEditor::render()
      {
        if (ImGui::CollapsingHeader("Tileset", ImGuiTreeNodeFlags_DefaultOpen))
        {
        int w_in_tiles = m_texture_w / m_tile_dimensions[0];
        int h_in_tiles = m_texture_h / m_tile_dimensions[1];
        
        ImGui::Dummy(ImVec2(0.0f, 3.0f));
        if (ImGui::InputInt("   ID", &m_selected_tile_id))
        {
          has_changed_id = true;
          if (m_selected_tile_id < 0) m_selected_tile_id = 0;
          else if (m_selected_tile_id > w_in_tiles*h_in_tiles - 1) m_selected_tile_id = w_in_tiles*h_in_tiles - 1;
        }

        ImGui::Dummy(ImVec2(0.0f, 3.0f));
        ImGui::PushID("tileset-slider");
        ImGui::SliderInt2("   Size", m_tile_dimensions, 2, 128);
        ImGui::PopID();
        ImGui::Dummy(ImVec2(0.0f, 3.0f));
        //ImGui::RadioButton("", bool active);
        ImGui::Checkbox("   Collidable", &m_tile_collidable);
        ImGui::SameLine();
        ImGui::Checkbox("   Scriptable", &m_tile_scriptable);
        ImGui::Dummy(ImVec2(0.0f, 3.0f));

        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.3f, 0.3f, 0.3f, 0.7f));

        for (int j = 0; j < h_in_tiles; ++j)
        {
            for (int i = 0; i < w_in_tiles; ++i)
            {
              int frame = 1;
              ImVec4 tint_col = ImVec4(1.f, 1.f, 1.f, 0.7f);
              int item_spacing = 0.f;
              ImVec2 size = ImVec2(m_tile_dimensions[0], m_tile_dimensions[1]);
              ImVec2 uv1 = ImVec2(i*m_tile_dimensions[0]/(float)m_texture_w, j*m_tile_dimensions[1]/(float)m_texture_h);
              ImVec2 uv2 = ImVec2((m_tile_dimensions[0] + i*m_tile_dimensions[0])/(float)m_texture_w, (m_tile_dimensions[1] + j*m_tile_dimensions[1])/(float)m_texture_h);
              ImTextureID texture = (void*)(intptr_t)m_texture.GetID();

              if (i + j*w_in_tiles == m_selected_tile_id)
              {
                tint_col = ImVec4(1.f, 1.f, 1.f, 1.f);
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.8f, 0.8f, 0.85f));

                // If user changed ID manually via the input, set the info about the new tile
                if (has_changed_id)
                {
                  has_changed_id = false;
                  m_selected_tile_texture = texture;
                  m_selected_tile_size = size;
                  m_selected_tile_uv1 = uv1;
                  m_selected_tile_uv2 = uv2;
                }
              }

              // Draw all tiles side by side
                ImGui::PushID(i + j*w_in_tiles);
                if(ImGui::ImageButton(texture, size, uv1, uv2, frame, ImVec4(0,0,0,0), tint_col))
                {
                    m_selected_tile_id = j*w_in_tiles + i;
                    m_selected_tile_texture = texture;
                    m_selected_tile_size = size;
                    m_selected_tile_uv1 = uv1;
                    m_selected_tile_uv2 = uv2;
                }

                // Pop Style color if we have the active tile
                if (ImGui::GetStyleColorVec4(ImGuiCol_Button).w == 0.85f)
                {
                  ImGui::PopStyleColor();
                }

                ImGui::PopID();
                ImGui::SameLine(0.0, item_spacing);
            }
            ImGui::NewLine();
        }
        ImGui::Dummy(ImVec2(0.0f, 12.0f));
        ImGui::PopStyleColor();
        ImGui::PopStyleVar();
        }
      }

      ImVec2 TilesetEditor::pos2tile(const int x, const int y)
      {
        return ImVec2(x/m_tile_dimensions[0], y/m_tile_dimensions[1]);
      }

      ImVec2 TilesetEditor::get_tile_dimensions()
      {
        return ImVec2(m_tile_dimensions[0], m_tile_dimensions[1]);
      }

      void TilesetEditor::render_tile_sprite(const ImVec2& pos, const float alpha)
      {
        if (m_selected_tile_texture != nullptr)
        {
          ImGui::SetNextWindowSize(ImVec2(64.f, 64.f), ImGuiCond_Always);
          ImGui::SetNextWindowPos(pos, ImGuiCond_Always);
          ImGui::SetNextWindowBgAlpha(alpha);
          ImGui::Begin("texture", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings);
          ImGui::Image(m_selected_tile_texture, m_selected_tile_size, m_selected_tile_uv1, m_selected_tile_uv2, ImVec4(1.f, 1.f, 1.f, alpha));
          ImGui::End();
        }
      }
}
}