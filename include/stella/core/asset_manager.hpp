#pragma once

#include "stella/core/asset.hpp"
#include "stella/core/json.hpp"
#include <string>
#include <map>

namespace stella
{
namespace core
{
  class AssetManager
  {
  public:
    AssetManager(const std::string& filepath);

    // Loads if the asset is not yet loaded and returns it
    template<typename... Args>
    void add (const std::string& name, const AssetType type, const std::string& filepath, Args... args);

    // Gets a loaded asset by its name
    std::shared_ptr<Asset>& get (const std::string& name, const AssetType type);

  private:
    std::string m_filepath;
    JSON m_json;
    std::map<std::string, std::shared_ptr<Asset>> m_assets;
    std::map<AssetType, std::shared_ptr<Asset>> m_null_assets;

  private:
    void m_init_null_assets();
    void m_init_assets();
  };
}
}
