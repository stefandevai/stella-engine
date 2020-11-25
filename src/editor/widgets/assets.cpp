#include "editor/widgets/assets.hpp"
#include "stella/core/asset_manager.hpp"

namespace editor
{
namespace widget
{

  Assets::Assets ()
  : Widget ("Assets")
  {
    m_open = true;
  }

  void Assets::render (stella::core::AssetManager& asset_manager)
  {
    if (!m_open)
    {
      return;
    }

    if (ImGui::Begin (m_title_string.c_str(), &m_open))
    {
      std::vector <std::string> textures;
      std::vector <std::string> models;
      std::vector <std::string> shaders;

      for (const auto& asset : asset_manager.m_assets)
      {
        auto type = asset.second.second->get_type();

        switch (type)
        {
          case stella::core::AssetType::TEXTURE:
            {
              textures.emplace_back (asset.first);
            }
            break;

          case stella::core::AssetType::MODEL:
            {
              models.emplace_back (asset.first);
            }
            break;

          case stella::core::AssetType::SHADER:
            {
              shaders.emplace_back (asset.first);
            }
            break;

          default:
            break;
        }
      }

      m_render_asset_list ("Textures", textures);
      m_render_asset_list ("Models", models);
      m_render_asset_list ("Shaders", shaders);

      ImGui::Dummy(ImVec2(0.0f, 2.0f));

      static bool show_add_asset = false;
      if (ImGui::Button("Add Asset###assets-button-1"))
      {
        show_add_asset = true;
      }

      if (show_add_asset)
      {
        if (ImGui::Begin("Add Asset###assets-window-2", &show_add_asset, ImGuiWindowFlags_NoDocking))
        {
          static char asset_name[128] = "";
          static char asset_type_name[128] = "";
          static stella::core::AssetType asset_type = stella::core::AssetType::NONE;
          ImGui::Text("Name:");
          ImGui::InputText("###input-add-asset-1", asset_name, IM_ARRAYSIZE(asset_name));
          ImGui::Text("Type:");

          if (ImGui::BeginCombo("###input-add-asset-2", asset_type_name, 0))
          {
            for (auto& asset_tag : asset_manager.m_asset_types)
            {
              const bool selected = (asset_type == asset_tag.second);
              if (ImGui::Selectable((asset_tag.first + "###asset-tag-" + asset_tag.first).c_str(), selected))
              {
                strcpy(asset_type_name, asset_tag.first.c_str());
                asset_type = asset_tag.second;
              }
            }
            ImGui::EndCombo();
          }

          switch (asset_type)
          {
            case stella::core::AssetType::TEXTURE:
              {
                ImGui::Text ("TODO Texture");
              }
              break;

            case stella::core::AssetType::MODEL:
              {
                ImGui::Text ("TODO Model");
              }
              break;

            case stella::core::AssetType::SHADER:
              {
                ImGui::Text ("TODO Shader");
              }
              break;

            default:
              break;
          }

          ImGui::Dummy(ImVec2(0.0f, 2.0f));

          if (ImGui::Button("Confirm###assets-button-2") && asset_name[0] != 0 && asset_type != stella::core::AssetType::NONE)
          {
            show_add_asset = false;
          }

          ImGui::SameLine();

          if (ImGui::Button("Cancel###assets-button-3"))
          {
            show_add_asset = false;
          }
        }
        ImGui::End();
      }

    }
    ImGui::End();
  }

  void Assets::m_render_asset_list (const std::string& title, const std::vector<std::string>& names)
  {
    if (ImGui::CollapsingHeader((title + "###dropdown-assets-" + title).c_str()))
    {
      for (const auto& name : names)
      {
        ImGui::Text ("%s", name.c_str());
      }
    }
  }
} // namespace widget
} // namespace stella

