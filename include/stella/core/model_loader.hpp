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
  class ModelLoader : public AssetLoader
  {
  public:
    ModelLoader (const std::string& filepath) : AssetLoader(AssetType::MODEL), m_filepath(filepath) {}
    virtual std::shared_ptr<Asset> construct() { return std::make_shared<ModelAsset>(m_filepath); }

  private:
    const std::string m_filepath;
  };

  
}
}

