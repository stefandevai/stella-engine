#include "stella/core/asset_manager.hpp"
#include "stella/core/texture_asset.hpp"
#include "stella/core/model_asset.hpp"
#include "stella/core/sound_asset.hpp"

#include <spdlog/spdlog.h>

namespace stella
{
namespace core
{
  AssetManager::AssetManager(const std::string& filepath)
    : m_filepath(filepath)
  {
    spdlog::info("HEREEEEEEEEEEE");
    //m_json.load (filepath);
    //m_init_null_assets ();
    //m_init_assets ();
  }

  // Loads if the asset is not yet loaded and returns it
  template <typename... Args>
  void AssetManager::add (const std::string& name, const AssetType type, const std::string& filepath, Args... args)
  {
    std::shared_ptr<Asset> asset;
    switch (type)
    {
      case AssetType::TEXTURE:
        {
          asset = std::make_shared<TextureAsset>(filepath, args...);
        }
        break;

      case AssetType::SOUND:
        {
          asset = std::make_shared<SoundAsset>(filepath, args...);
        }
        break;

      case AssetType::MODEL:
        {
          asset = std::make_shared<ModelAsset>(filepath, args...);
        }
        break;
      default:
        break;
    }
    m_assets.insert({ name, asset });
  }

  // Gets a loaded asset by its name
  std::shared_ptr<Asset>& AssetManager::get (const std::string& name, const AssetType type)
  {
    try
    {
      auto& asset = m_assets.at(name);
      return asset;
    }
    catch (std::out_of_range& e)
    {
      spdlog::warn ("There's no asset named {}.\n{}", name, e.what());
      return m_null_assets.at(type);
    }
  }

  void AssetManager::m_init_null_assets()
  {

  }

  void AssetManager::m_init_assets()
  {

  }
}
}
