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
        int m_selected_tile_id = -1;

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
        ImGui::BeginChild("Tileset Editor",
                            ImVec2(0.f, 0.f),
                            true,
                            ImGuiWindowFlags_ChildWindow);
        
        ImGui::Dummy(ImVec2(0.0f, 3.0f));
        ImGui::SliderInt2("  Tile dimensions", m_tile_dimensions, 2, 128);
        ImGui::Dummy(ImVec2(0.0f, 3.0f));

        int w_in_tiles = m_texture_w / m_tile_dimensions[0];
        int h_in_tiles = m_texture_h / m_tile_dimensions[1];

        for (int j = 0; j < h_in_tiles; ++j)
        {
            for (int i = 0; i < w_in_tiles; ++i)
            {
                ImGui::PushID(i + j*w_in_tiles);
                if(ImGui::ImageButton((void*)(intptr_t)m_texture.GetID(),
                    ImVec2(m_tile_dimensions[0], m_tile_dimensions[1]),
                    ImVec2(i*m_tile_dimensions[0]/(float)m_texture_w, j*m_tile_dimensions[1]/(float)m_texture_h),
                    ImVec2((m_tile_dimensions[0] + i*m_tile_dimensions[0])/(float)m_texture_w, (m_tile_dimensions[1] + j*m_tile_dimensions[1])/(float)m_texture_h),
                    0,
                    ImVec4(0,0,0,0),
                    ImVec4(1.f,1.f,1.f,0.7)))
                {
                    m_selected_tile_id = j*w_in_tiles + i;
                };
                ImGui::PopID();
                ImGui::SameLine();
            }
            ImGui::NewLine();
        }
          
        ImGui::EndChild();
      }

      ImVec2 pos2tile(const int x, const int y)
      {
        return ImVec2(x/m_tile_dimensions[0], y/m_tile_dimensions[1]);
      }

  };
}
}

