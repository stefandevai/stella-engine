#pragma once

#include "stella/core/asset.hpp"
#include "stella/core/json.hpp"
#include <string>
#include <filesystem>
#include <unordered_map>

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
    std::shared_ptr<Asset> get (const std::string& name);

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
