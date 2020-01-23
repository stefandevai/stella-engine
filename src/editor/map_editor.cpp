#include "editor/map_editor.h"
#include "../../nikte/game.h"

namespace stella
{
namespace editor
{
    MapEditor::MapEditor(nikte::Game& game)
      : m_tile_map(game.m_tile_map)
    {
      this->reset_map_settings();
    }

    MapEditor::~MapEditor() {}

    void MapEditor::render()
    { 
      if (ImGui::CollapsingHeader("Map"))
      {
        ImGui::Dummy(ImVec2(0.f, 3.f));

        ImGui::InputText(" Name", m_map_name, IM_ARRAYSIZE(m_map_name));
        ImGui::Dummy(ImVec2(0.f, 3.f));
        ImGui::InputTextWithHint(" Path", "Map file location.", m_path, IM_ARRAYSIZE(m_path));
        ImGui::Dummy(ImVec2(0.f, 3.f));
        ImGui::PushID("map-slider");
        ImGui::SliderInt2(" Size", m_map_size, min_map_size, max_map_size);
        ImGui::PopID();
        ImGui::Dummy(ImVec2(0.f, 3.f));

        // Layers
        if (ImGui::TreeNodeEx("Layers", ImGuiTreeNodeFlags_DefaultOpen))
        {
        ImGui::BeginGroup();
        for (auto it = m_tile_map.layers.rbegin(); it != m_tile_map.layers.rend(); ++it)
        //for (auto& layer : m_tile_map.layers)
        {
          auto& layer = *it;
          bool selected = false;
          if (layer->get_id() == get_selected_layer_id())
          {
            selected = true;
          }

          ImGui::PushID(layer->get_id());
          if(ImGui::Selectable(layer->get_name().c_str(), selected))
          {
            m_selected_layer = layer;
          }
          ImGui::PopID();
        }
        ImGui::EndGroup();
        ImGui::TreePop();
        }
        ImGui::Dummy(ImVec2(0.0f, 12.0f));
      }
    }

    void MapEditor::reset_map_settings()
    {
      m_selected_layer = m_tile_map.layers.front();
      std::size_t length = m_tile_map.get_name().copy(m_map_name, 128);
      m_map_name[length] = '\0';
      length = m_tile_map.get_path().copy(m_path, 128);
      m_path[length] = '\0';
      m_map_size[0] = (int)m_tile_map.width();
      m_map_size[1] = (int)m_tile_map.height();
    }

    void MapEditor::update_map_settings()
    {
      m_tile_map.set_name(m_map_name);
    }
}
}