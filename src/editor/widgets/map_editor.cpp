#include "editor/widgets/map_editor.h"
#include "../../nikte/game.h"

namespace stella
{
namespace widget
{
  MapEditor::MapEditor (nikte::Game& game) : Widget("Map Editor"), m_game (game), m_tile_map (game.m_tile_map)
  {
    this->reset_map_settings();
  }

  void MapEditor::render()
  {
    if (ImGui::Begin (m_name.c_str(), &m_open))
    {
      const float item_width = ImGui::CalcItemWidth();

      // Map Name
      ImGui::Dummy (ImVec2 (0.f, 3.f));
      ImGui::Text ("Map name:");
      ImGui::PushID ("map-input#1");
      ImGui::InputText ("", m_map_name, IM_ARRAYSIZE (m_map_name));
      ImGui::PopID();
      ImGui::Dummy (ImVec2 (0.f, 3.f));

      // Map file location
      ImGui::Text ("Map file location:");
      ImGui::PushItemWidth (item_width - 64.f);
      ImGui::PushID ("map-input#2");
      ImGui::InputTextWithHint ("", "Map file location.", m_path, IM_ARRAYSIZE (m_path));
      ImGui::PopID();
      ImGui::PopItemWidth();
      ImGui::SameLine (0.f, 4.f);
      ImGui::Button ("...", ImVec2 (60.f, 0));
      ImGui::Dummy (ImVec2 (0.f, 3.f));
      ImGui::Separator();

      // Map dimensions
      ImGui::Text ("Map dimensions: %d x %d (top, right, bottom, left)", m_map_width, m_map_height);
      ImGui::PushItemWidth (item_width - 64.f);
      ImGui::PushID ("map-slider");
      if (ImGui::InputInt4 ("", m_map_size))
      {
        for (int i = 0; i < 4; ++i)
        {
          if (m_map_size[i] < MIN_MAP_SIZE)
          {
            m_map_size[i] = MIN_MAP_SIZE;
          }
          else if (m_map_size[i] > MAX_MAP_SIZE)
          {
            m_map_size[i] = MAX_MAP_SIZE;
          }
        }
      }

      ImGui::PopID();
      ImGui::PopItemWidth();
      ImGui::SameLine (0.f, 4.f);
      if (ImGui::Button ("Resize", ImVec2 (60.f, 0)))
      {
        m_tile_map.resize (m_map_size[0], m_map_size[1], m_map_size[2], m_map_size[3]);
        m_map_width += m_map_size[1] + m_map_size[3];
        m_map_height += m_map_size[0] + m_map_size[2];
        m_map_size[0] = m_map_size[1] = m_map_size[2] = m_map_size[3] = 0;

        m_game.m_script_api.set_variable<int> ("e_map_width", m_map_width);
        m_game.m_script_api.set_variable<int> ("e_map_height", m_map_height);
      }
      ImGui::Dummy (ImVec2 (0.f, 3.f));
      ImGui::Separator();

      // Layers
      if (ImGui::TreeNodeEx ("Layers", ImGuiTreeNodeFlags_DefaultOpen))
      {
        ImGui::BeginGroup();
        for (auto it = m_tile_map.layers.rbegin(); it != m_tile_map.layers.rend(); ++it)
        // for (auto& layer : m_tile_map.layers)
        {
          auto& layer   = *it;
          bool selected = false;
          if (layer->get_id() == get_selected_layer_id())
          {
            selected = true;
          }

          ImGui::PushID (layer->get_id());
          if (ImGui::Selectable (layer->get_name().c_str(), selected))
          {
            m_selected_layer = layer;
          }
          ImGui::PopID();
        }
        ImGui::EndGroup();
        ImGui::TreePop();
      }
      ImGui::Dummy (ImVec2 (0.0f, 12.0f));
    }
    ImGui::End();
  }

  void MapEditor::reset_map_settings()
  {
    m_selected_layer   = m_tile_map.layers.front();
    std::size_t length = m_tile_map.get_name().copy (m_map_name, 128);
    m_map_name[length] = '\0';
    length             = m_tile_map.get_path().copy (m_path, 128);
    m_path[length]     = '\0';
    m_map_width        = (int) m_tile_map.width();
    m_map_height       = (int) m_tile_map.height();
  }

  void MapEditor::update_map_settings() { m_tile_map.set_name (m_map_name); }
} // namespace widget
} // namespace stella