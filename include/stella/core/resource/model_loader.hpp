#pragma once

#include "stella/core/resource/asset.hpp"

namespace stella
{
namespace core
{
  class AssetManager;

  class ModelLoader : public AssetLoader
  {
  public:
    ModelLoader (const std::string& filepath, AssetManager* asset_manager);
    virtual std::shared_ptr<Asset> construct();

  private:
    const std::string m_filepath;
    AssetManager* m_asset_manager;
  };

} // namespace core
} // namespace stella
