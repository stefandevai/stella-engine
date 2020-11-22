#pragma once

#include "stella/core/asset.hpp"
#include "stella/graphics/texture2.hpp"

namespace stella
{
namespace core
{
  // TODO: Add proper asset creation
  class TextureLoader : public AssetLoader
  {
    public:
      TextureLoader (const std::string& filepath) : AssetLoader (AssetType::TEXTURE), m_filepath (filepath) {}
      virtual std::shared_ptr<Asset> construct() { return std::make_shared<graphics::Texture>(m_filepath); }

    private:
      const std::string m_filepath;
  };

}
}
