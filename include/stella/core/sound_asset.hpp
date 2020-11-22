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
  struct SoundLoader : public AssetLoader
  {
    SoundLoader (const std::string& filepath) : AssetLoader(filepath, AssetType::SOUND) {}
    virtual std::shared_ptr<Asset> construct() { return std::make_shared<SoundAsset>(filepath); }
  };
  
}
}

