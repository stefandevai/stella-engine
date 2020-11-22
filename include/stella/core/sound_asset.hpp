#pragma once

#include "stella/core/asset.hpp"

namespace stella
{
namespace core
{
  class SoundAsset : public Asset
  {
    public:
      SoundAsset (const std::string& filepath) {}

  };

  // TODO: Add proper asset creation
  struct SoundData : public AssetData
  {
    SoundData (const std::string& filepath) : AssetData(filepath, AssetType::SOUND) {}
    virtual std::shared_ptr<Asset> construct() { return std::make_shared<SoundAsset>(filepath); }
  };
  
}
}

