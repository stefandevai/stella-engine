#pragma once

#include "stella/core/asset.hpp"
#include "stella/graphics/mesh.hpp"

namespace stella
{
namespace core
{

  class AssetManager;

  class ModelAsset : public Asset
  {
    public:
      const std::vector<std::shared_ptr<graphics::Mesh>> meshes;

    public:
      ModelAsset (const std::vector<std::shared_ptr<graphics::Mesh>>& meshes) : meshes (meshes) {}
  };

  // TODO: Add proper asset creation
  class ModelLoader : public AssetLoader
  {
  public:
    ModelLoader (const std::string& filepath, AssetManager* asset_manager);
    virtual std::shared_ptr<Asset> construct();

  private:
    const std::string m_filepath;
    AssetManager* m_asset_manager;
  };

  
}
}

