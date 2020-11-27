#include "stella/core/resource/asset_manager.hpp"
#include "stella/core/resource/texture_loader.hpp"
#include "stella/core/resource/model_loader.hpp"
#include "stella/core/resource/shader_loader.hpp"
#include "../../../lib/json/json.hpp"

namespace stella
{
namespace core
{
  AssetManager::AssetManager (const std::filesystem::path& filepath) : m_filepath (filepath)
  {
    // Get base dir from the config filepath
    m_base_dir = filepath.string();
    m_base_dir.remove_filename();

    // Load config file
    m_json.load (m_filepath);
    m_init_assets();
  }

  template<typename T, typename... Args>
  void AssetManager::add (const std::string& id, const std::string& filepath, Args... args)
  {
    // TODO: Better handling of existing textures
    // If the key already exists just return
    if (m_assets.find (id) != m_assets.end())
    {
      return;
    }

    auto asset_loader = std::make_unique<T> (m_base_dir / filepath, args...);
    std::weak_ptr<Asset> asset_ptr;
    assert (asset_loader != nullptr);
    assert (asset_ptr != nullptr);
    m_assets.emplace (id, std::make_pair (std::move (asset_ptr), std::move (asset_loader)));
  }

  void AssetManager::m_init_assets()
  {
    if (m_json.object[ASSET_OBJECT_ASSETS] == nullptr)
    {
      spdlog::warn ("Project has no assets.");
      return;
    }

    auto assets = m_json.object[ASSET_OBJECT_ASSETS].get<std::vector<nlohmann::json>>();

    for (auto& asset_info : assets)
    {
      const auto id       = asset_info[ASSET_PARAMETER_ID].get<std::string>();
      const auto type_tag = asset_info[ASSET_PARAMETER_TYPE].get<std::string>();
      AssetType type      = AssetType::NONE;

      try
      {
        type = m_asset_types.at (type_tag);
      }
      catch (std::out_of_range& e)
      {
        spdlog::warn ("Unknown asset type {}.\n{}", type_tag, e.what());
        continue;
      }

      switch (type)
      {
        case AssetType::TEXTURE:
        {
          const auto filepath = asset_info[ASSET_PARAMETER_PATH].get<std::string>();
          // TODO: Parse all texture types
          // const auto texture_type = asset_info["textureType"].get<std::string>();
          const auto texture_type = graphics::TextureType::DIFFUSE;

          add<TextureLoader> (id, m_base_dir / filepath, texture_type);
        }
        break;

        case AssetType::MODEL:
        {
          const auto filepath = asset_info[ASSET_PARAMETER_PATH].get<std::string>();
          add<ModelLoader> (id, m_base_dir / filepath, this);
        }
        break;

        case AssetType::SHADER:
        {
          const auto filepath = asset_info[ASSET_PARAMETER_PATH].get<std::string>();
          add<ShaderLoader> (id, m_base_dir / filepath);
        }
        break;

        case AssetType::TEXTURE_ATLAS:
        {
          const auto filepath = asset_info[ASSET_PARAMETER_PATH].get<std::string>();
          const auto uniform  = asset_info[ASSET_PARAMETER_UNIFORM].get<bool>();
          // TODO: Parse all texture types
          // const auto texture_type = asset_info["textureType"].get<std::string>();
          const auto texture_type = graphics::TextureType::DIFFUSE;

          if (uniform)
          {
            const auto horizontal_frames = asset_info[ASSET_PARAMETER_HORIZONTAL_FRAMES].get<int>();
            const auto vertical_frames   = asset_info[ASSET_PARAMETER_VERTICAL_FRAMES].get<int>();

            add<TextureLoader> (id, filepath, texture_type, horizontal_frames, vertical_frames);
          }
          else
          {
            // TODO: Implement functionality for non uniform texture atlases
          }
        }
        break;

        default:
          spdlog::warn ("Unknown asset type {}", type_tag);
          break;
      }
    }
  }
} // namespace core
} // namespace stella
