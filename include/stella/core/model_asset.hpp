#pragma once

#include "stella/core/asset.hpp"

namespace stella
{
namespace core
{
  class ModelAsset : public Asset
  {
    public:
      ModelAsset (const std::string& filepath) {}

  };

  // TODO: Add proper asset creation
  struct ModelLoader : public AssetLoader
  {
    ModelLoader (const std::string& filepath) : AssetLoader(filepath, AssetType::MODEL) {}
    virtual std::shared_ptr<Asset> construct() { return std::make_shared<ModelAsset>(filepath); }
  };

  
}
}

