#include "editor/widgets/map_editor.hpp"
#include "editor/widgets/file_dialog.hpp"
#include "../../nikte/game.hpp"

namespace editor
{
namespace widget
{
  const std::string MapEditor::SAVE_DIALOG_ID = "SaveMapDgl";
  const std::string MapEditor::LOAD_DIALOG_ID = "LoadMapDgl";

  MapEditor::MapEditor (nikte::Game& game) : Widget ("Map Editor"), m_game (game), m_tile_map (game.m_tile_map)
  {
    this->reset_map_settings();
    m_open = true;
  }

  void MapEditor::render()
  {
    if (m_open)
    {
      if (ImGui::Begin (m_name.c_str(), &m_open))
      {
        // if (m_tile_map.has_loaded())
        // {
        const float item_width = ImGui::CalcItemWidth();

        // Map Name
        ImGui::Dummy (ImVec2 (0.f, 3.f));
        ImGui::Text ("Map name:");
        ImGui::PushID ("map-input#1");
        ImGui::InputTextWithHint ("", "Give a name...", m_map_name, IM_ARRAYSIZE (m_map_name));
        ImGui::PopID();
        ImGui::Dummy (ImVec2 (0.f, 3.f));

        // Map file location
        ImGui::Text ("Map file location:");
        ImGui::PushItemWidth (item_width - 64.f);
        ImGui::PushID ("map-input#2");
        ImGui::InputTextWithHint ("", "Choose a path...", m_path, IM_ARRAYSIZE (m_path));
        ImGui::PopID();
        ImGui::PopItemWidth();
        ImGui::SameLine (0.f, 4.f);
        if (ImGui::Button ("...", ImVec2 (60.f, 0)))
        {
          load();
        }
        ImGui::Dummy (ImVec2 (0.f, 3.f));
        ImGui::Separator();

        // Map dimensions
        ImGui::Text ("Map dimensions: %d x %d (top, right, bottom, left)", m_tile_map.width(), m_tile_map.height());
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
          m_map_size[0] = m_map_size[1] = m_map_size[2] = m_map_size[3] = 0;

          m_game.m_script_api.set_variable<int> ("e_map_width", m_tile_map.width());
          m_game.m_script_api.set_variable<int> ("e_map_height", m_tile_map.height());
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
            if (get_selected_layer_id() != -1 && layer->get_id() == get_selected_layer_id())
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
          if (ImGui::Button ("Create Layer"))
          {
            m_tile_map.create_layer ("tileset");
          }
          ImGui::EndGroup();
          ImGui::TreePop();
        }
        ImGui::Dummy (ImVec2 (0.0f, 12.0f));
      }
      // else
      // {
      //   ImGui::Text ("Create a new map or load an existing one.");
      // }
      // }
      ImGui::End();
    }
  }

  void MapEditor::reset_map_settings()
  {
    if (m_tile_map.has_loaded())
    {
      m_selected_layer   = m_tile_map.layers.front();
      std::size_t length = m_tile_map.get_name().copy (m_map_name, 128);
      m_map_name[length] = '\0';
      length             = m_tile_map.get_path().copy (m_path, 128);
      m_path[length]     = '\0';
    }
    else
    {
      m_selected_layer = nullptr;
      memset (m_map_name, 0, sizeof m_map_name);
      memset (m_path, 0, sizeof m_path);
    }
  }

  void MapEditor::update_map_settings() { m_tile_map.set_name (m_map_name); }

  void MapEditor::load()
  {
    ImGuiFileDialog::Instance()->OpenModal (LOAD_DIALOG_ID, "Choose File", ".xml\0.lua\0\0", ".");
  }

  void MapEditor::save()
  {
    update_map_settings();
    m_tile_map.save (m_map_name);
  }

  void MapEditor::save_as() { ImGuiFileDialog::Instance()->OpenModal (SAVE_DIALOG_ID, "Save As...", ".xml\0\0", "."); }

  void MapEditor::render_file_dialog()
  {
    ImGui::PushStyleVar (ImGuiStyleVar_WindowRounding, 0.0);
    if (ImGuiFileDialog::Instance()->FileDialog (LOAD_DIALOG_ID,
                                                 ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDocking))
    {
      ImGui::SetWindowFocus();
      if (ImGuiFileDialog::Instance()->IsOk == true)
      {
        std::string file_path_name = ImGuiFileDialog::Instance()->GetFilepathName();
        m_tile_map.load (file_path_name);
        reset_map_settings();
      }
      ImGuiFileDialog::Instance()->CloseDialog (LOAD_DIALOG_ID);
    }

    if (ImGuiFileDialog::Instance()->FileDialog (SAVE_DIALOG_ID,
                                                 ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDocking))
    {
      ImGui::SetWindowFocus();
      if (ImGuiFileDialog::Instance()->IsOk == true)
      {
        std::string file_path_name = ImGuiFileDialog::Instance()->GetFilepathName();
        m_tile_map.save (file_path_name);
        m_tile_map.load (file_path_name);
        reset_map_settings();
      }
      ImGuiFileDialog::Instance()->CloseDialog (SAVE_DIALOG_ID);
    }
    ImGui::PopStyleVar();
  }
} // namespace widget
} // namespace editor
