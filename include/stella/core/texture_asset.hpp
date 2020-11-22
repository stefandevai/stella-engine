#pragma once

#include "stella/core/asset.hpp"
#include "stella/graphics/texture2.hpp"

namespace stella
{
namespace core
{

  // TODO: Add proper asset creation
  struct TextureLoader : public AssetLoader
  {
    TextureLoader (const std::string& filepath) : AssetLoader(filepath, AssetType::TEXTURE) {}
    virtual std::shared_ptr<Asset> construct() { return std::make_shared<graphics::Texture>(filepath); }
  };

}
}
