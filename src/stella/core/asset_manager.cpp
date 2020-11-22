#include "stella/core/asset_manager.hpp"
#include "stella/core/texture_asset.hpp"
#include "stella/core/model_asset.hpp"
#include "stella/core/sound_asset.hpp"

#include "../../../lib/json/json.hpp"
#include <spdlog/spdlog.h>

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
    auto asset_data = std::make_unique<T>(filepath, args...);
    assert(asset_data != nullptr);
    m_assets.emplace(name, std::make_pair(std::weak_ptr<Asset>(), std::move(asset_data)));
  }

  template <typename T>
  std::shared_ptr<T> AssetManager::get (const std::string& name)
  {
    try
    {
      auto& asset_pair = m_assets.at(name);
      auto& asset_ptr = asset_pair.first;
      if (asset_ptr.expired())
      {
        // TODO: Load resource
        auto asset = asset_pair.second->construct();
        asset_ptr = asset;
        return asset;
      }
      return asset_ptr.lock();
    }
    catch (std::out_of_range& e)
    {
      spdlog::warn ("There's no asset named {}.\n{}", name, e.what());
      return nullptr;
    }
  }

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
      auto name = asset_info["name"].get<std::string>();
      auto type_tag = asset_info["type"].get<std::string>();
      auto filepath = asset_info["path"].get<std::string>();
      auto full_path = m_base_dir / filepath;
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
            add<TextureData>(name, full_path);
          }
          break;

        case AssetType::MODEL:
          {
            // TODO: Implement model loading
            add<ModelData>(name, full_path);
          }
          break;

        case AssetType::SOUND:
          {
            // TODO: Implement sound loading
            add<SoundData>(name, full_path);
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
