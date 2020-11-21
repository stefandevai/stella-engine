#include "stella/core/texture_asset.hpp"

namespace stella
{
namespace core
{

  TextureAsset::TextureAsset (const std::string& filepath)
    : Asset(AssetType::TEXTURE, filepath)
  {

  }
  
}
}

