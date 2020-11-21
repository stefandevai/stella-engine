#pragma once

#include "stella/core/asset.hpp"

namespace stella
{
namespace core
{

  class SoundAsset : public Asset
  {
    public:
      SoundAsset (const std::string& filepath) : Asset (AssetType::SOUND, filepath) {}

  };
  
}
}

