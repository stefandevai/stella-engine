#pragma once

#include <string>
#include <iostream>
#include <memory>

#include "./imgui/imgui.h"
#include "stella/graphics/texture.h"

namespace stella
{
namespace editor
{
  class TilesetEditor
  {
    private:
        std::string m_path;
        graphics::Texture m_texture;
        int m_tile_dimensions[2] = {32,32};
        int m_texture_w = 0, m_texture_h = 0;
        
        bool m_tile_collidable = false;
        bool m_tile_scriptable = false;
        int m_selected_tile_id = 0;
        ImTextureID m_selected_tile_texture = nullptr;
        ImVec2 m_selected_tile_size;
        ImVec2 m_selected_tile_uv1;
        ImVec2 m_selected_tile_uv2;

        bool has_changed_id = false;

    public:
      TilesetEditor(const std::string& path)
        : m_path(path), m_texture(path)
      {
        m_texture_w = m_texture.GetWidth();
        m_texture_h = m_texture.GetHeight();
      }

      ~TilesetEditor() {}

      int get_selected_tile_id()
      {
          return m_selected_tile_id;
      }
      
      void render()
      {
        int w_in_tiles = m_texture_w / m_tile_dimensions[0];
        int h_in_tiles = m_texture_h / m_tile_dimensions[1];

        ImGui::BeginChild("Tileset Editor",
                            ImVec2(0.f, 0.f),
                            true,
                            ImGuiWindowFlags_ChildWindow);
        
        ImGui::Dummy(ImVec2(0.0f, 3.0f));
        if (ImGui::InputInt("   Tile ID", &m_selected_tile_id))
        {
          has_changed_id = true;
          if (m_selected_tile_id < 0) m_selected_tile_id = 0;
          else if (m_selected_tile_id > w_in_tiles*h_in_tiles - 1) m_selected_tile_id = w_in_tiles*h_in_tiles;
        }

        ImGui::Dummy(ImVec2(0.0f, 3.0f));
        ImGui::SliderInt2("   Tile dimensions", m_tile_dimensions, 2, 128);
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

                ImGui::PushID(i + j*w_in_tiles);
                if(ImGui::ImageButton(texture, size, uv1, uv2, frame, ImVec4(0,0,0,0), tint_col))
                {
                    m_selected_tile_id = j*w_in_tiles + i;
                    m_selected_tile_texture = texture;
                    m_selected_tile_size = size;
                    m_selected_tile_uv1 = uv1;
                    m_selected_tile_uv2 = uv2;

                    // We push this var to avoid a mismatch
                    //ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.8f, 0.8f, 0.85f));
                }
                if (ImGui::GetStyleColorVec4(ImGuiCol_Button).w == 0.85f)
                //if (i + j*w_in_tiles == m_selected_tile_id)
                {
                  ImGui::PopStyleColor();
                }

                ImGui::PopID();
                ImGui::SameLine(0.0, item_spacing);
            }
            ImGui::NewLine();
        }
        ImGui::PopStyleColor();
        ImGui::PopStyleVar();
        ImGui::EndChild();
      }

      ImVec2 pos2tile(const int x, const int y)
      {
        return ImVec2(x/m_tile_dimensions[0], y/m_tile_dimensions[1]);
      }

      ImVec2 get_tile_dimensions()
      {
        return ImVec2(m_tile_dimensions[0], m_tile_dimensions[1]);
      }

      void render_tile_sprite(const ImVec2& pos, const float alpha = 1.f)
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
  };
}
}

