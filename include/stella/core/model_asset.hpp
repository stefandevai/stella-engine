#pragma once

#include "stella/core/asset.hpp"

namespace stella
{
namespace core
{

  class ModelAsset : public Asset
  {
    public:
      ModelAsset (const std::string& filepath) : Asset (AssetType::MODEL, filepath) {}

  };
  
}
}

