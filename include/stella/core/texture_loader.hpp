#pragma once

#include "stella/core/asset.hpp"
#include "stella/graphics/texture2.hpp"

namespace stella
{
namespace core
{

  class TextureLoader : public AssetLoader
  {
    public:
      TextureLoader (const std::string& filepath, const graphics::TextureType type) : AssetLoader (AssetType::TEXTURE), m_filepath (filepath), m_type (type) {}
      virtual std::shared_ptr<Asset> construct() { return std::make_shared<graphics::Texture>(m_filepath, m_type); }

    private:
      const std::string m_filepath;
      const graphics::TextureType m_type;
  };

}
}
