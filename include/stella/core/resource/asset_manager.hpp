#pragma once

#include "stella/core/resource/asset.hpp"
#include "stella/core/resource/asset_parameters.hpp"
#include "stella/core/json.hpp"
#include <string>
#include <filesystem>
#include <unordered_map>
#include <spdlog/spdlog.h>

namespace editor
{
namespace widget
{
  class Assets;
}
} // namespace editor

namespace stella
{
namespace core
{
  using AssetPair = std::pair<std::weak_ptr<Asset>, std::unique_ptr<AssetLoader>>;
  using AssetMap  = std::unordered_map<std::string, AssetPair>;

  class AssetManager
  {
  public:
    AssetManager (const std::filesystem::path& filepath);

    // Add resource data to the internal map
    template<class T, typename... Args>
    void add (const std::string& name, const std::string& filepath, Args... args);

    // Gets a loaded asset by its name
    template<typename T>
    std::shared_ptr<T> get (const std::string& id)
    {
      std::shared_ptr<T> asset_value = nullptr;

      try
      {
        auto& asset_pair = m_assets.at (id);
        auto& asset_ptr  = asset_pair.first;

        if (asset_ptr.expired())
        {
          asset_value = std::dynamic_pointer_cast<T> (asset_pair.second->construct());
          asset_ptr   = asset_value;
        }
        else
        {
          asset_value = std::dynamic_pointer_cast<T> (asset_ptr.lock());
        }
        assert (asset_value != nullptr && "Template type must be an Asset type");
      }
      catch (std::out_of_range& e)
      {
        // TODO: Add default asset to return
        spdlog::warn ("There's no asset with ID {}.\n{}", id, e.what());
        return nullptr;
      }

      return asset_value;
    }

  private:
    const std::filesystem::path m_filepath;
    std::filesystem::path m_base_dir;
    JSON m_json;
    AssetMap m_assets;

    const std::unordered_map<std::string, AssetType> m_asset_types = {
        {ASSET_TYPE_TEXTURE, AssetType::TEXTURE},
        {ASSET_TYPE_TEXTURE_ATLAS, AssetType::TEXTURE_ATLAS},
        {ASSET_TYPE_SHADER, AssetType::SHADER},
        {ASSET_TYPE_MODEL, AssetType::MODEL},
    };

#ifdef STELLA_BUILD_EDITOR
    friend class ::editor::widget::Assets;
#endif

  private:
    void m_init_assets();
  };
} // namespace core
} // namespace stella
