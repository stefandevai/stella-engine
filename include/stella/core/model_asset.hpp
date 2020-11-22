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
  struct ModelData : public AssetData
  {
    ModelData (const std::string& filepath) : AssetData(filepath, AssetType::MODEL) {}
    virtual std::shared_ptr<Asset> construct() { return std::make_shared<ModelAsset>(filepath); }
  };

  
}
}

