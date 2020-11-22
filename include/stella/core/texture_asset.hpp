#pragma once

#include "stella/core/asset.hpp"
#include "stella/graphics/texture2.hpp"

namespace stella
{
namespace core
{

  class TextureAsset : public Asset
  {
  public:
    TextureAsset (const std::string& filepath);

  private:
    std::shared_ptr<graphics::Texture> m_data;
  };
  
  // TODO: Add proper asset creation
  struct TextureData : public AssetData
  {
    TextureData (const std::string& filepath) : AssetData(filepath, AssetType::TEXTURE) {}
    virtual std::shared_ptr<Asset> construct() { return std::make_shared<TextureAsset>(filepath); }
  };

}
}
