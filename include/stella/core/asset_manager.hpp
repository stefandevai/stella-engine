#pragma once

#include "stella/core/asset.hpp"
#include "stella/core/json.hpp"
#include <string>
#include <filesystem>
#include <unordered_map>
#include <spdlog/spdlog.h>

namespace stella
{
namespace core
{
  using AssetPair = std::pair<std::weak_ptr<Asset>, std::unique_ptr<AssetLoader>>;
  using AssetMap = std::unordered_map<std::string, AssetPair>;

  class AssetManager
  {
  public:
    AssetManager(const std::filesystem::path& filepath);

    // Add resource data to the internal map
    template <class T, typename... Args>
    void add (const std::string& name, const std::string& filepath, Args... args);

    // Gets a loaded asset by its name
    template <typename T>
    std::shared_ptr<T> get (const std::string& name)
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
          spdlog::critical("Getting new asset: {}", name);
          return std::dynamic_pointer_cast<T>(asset);
        }

        spdlog::critical("Getting existing asset: {}", name);
        return std::dynamic_pointer_cast<T>(asset_ptr.lock());
      }
      catch (std::out_of_range& e)
      {
        spdlog::warn ("There's no asset named {}.\n{}", name, e.what());
        return nullptr;
      }
    }

  private:
    const std::filesystem::path m_filepath;
    std::filesystem::path m_base_dir;
    JSON m_json;
    //std::unordered_map<std::string, std::shared_ptr<Asset>> m_assets;
    AssetMap m_assets;

    const std::unordered_map<std::string, AssetType> m_asset_types =
    {
      {"texture", AssetType::TEXTURE},
      {"model", AssetType::MODEL},
      {"shader", AssetType::SHADER},
    };

  private:
    void m_init_assets();
  };
}
}
