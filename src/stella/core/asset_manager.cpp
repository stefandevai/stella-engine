#include "stella/core/asset_manager.hpp"
#include "stella/core/texture_loader.hpp"
#include "stella/core/model_loader.hpp"
#include "stella/core/shader_loader.hpp"

#include "../../../lib/json/json.hpp"

namespace stella
{
namespace core
{

  AssetManager::AssetManager(const std::filesystem::path& filepath)
    : m_filepath(filepath)
  {
    // Get base dir from the config filepath
    m_base_dir = filepath.string();
    m_base_dir.remove_filename();

    // Load config file
    m_json.load (m_filepath);
    m_init_assets ();
  }

  template <typename T, typename... Args>
  void AssetManager::add (const std::string& name, const std::string& filepath, Args... args)
  {
    // TODO: Better handling of existing textures
    // If the key already exists just return
    if (m_assets.find(name) != m_assets.end())
    {
      return;
    }

    auto asset_loader = std::make_unique<T>(filepath, args...);
    std::weak_ptr<Asset> asset_ptr;
    assert (asset_loader != nullptr);
    assert (asset_ptr != nullptr);
    m_assets.emplace(name, std::make_pair(std::move(asset_ptr), std::move(asset_loader)));
  }

  //std::shared_ptr<Asset> AssetManager::get (const std::string& name)

  void AssetManager::m_init_assets()
  {
    if (m_json.object["assets"] == nullptr)
    {
      spdlog::warn("Project has no assets.");
      return;
    }

    auto assets = m_json.object["assets"].get<std::vector<nlohmann::json>>();
    
    for (auto& asset_info : assets)
    {
      const auto name = asset_info["name"].get<std::string>();
      const auto type_tag = asset_info["type"].get<std::string>();
      AssetType type = AssetType::NONE;

      try
      {
        type = m_asset_types.at(type_tag);
      }
      catch (std::out_of_range& e)
      {
        spdlog::warn("Unknown asset type {}.\n{}", type_tag, e.what());
        continue;
      }

      switch (type)
      {
        case AssetType::TEXTURE:
          {
            const auto filepath = asset_info["path"].get<std::string>();
            //const auto texture_type = asset_info["textureType"].get<std::string>();
            const auto full_path = m_base_dir / filepath;

            // TODO: Parse all texture types
            add<TextureLoader>(name, full_path, graphics::TextureType::DIFFUSE);
          }
          break;

        case AssetType::MODEL:
          {
            const auto filepath = asset_info["path"].get<std::string>();
            const auto full_path = m_base_dir / filepath;
            // TODO: Implement model loading
            add<ModelLoader>(name, full_path);
          }
          break;

        case AssetType::SHADER:
          {
            const auto vertex_filepath = asset_info["vertexFilepath"].get<std::string>();
            const auto fragment_filepath = asset_info["fragmentFilepath"].get<std::string>();
            // TODO: Implement sound loading
            add<ShaderLoader>(name, m_base_dir / vertex_filepath, m_base_dir / fragment_filepath);
          }
          break;

        default:
          spdlog::warn("Unknown asset type {}", type_tag);
          break;
      }
    }
  }
}
}
