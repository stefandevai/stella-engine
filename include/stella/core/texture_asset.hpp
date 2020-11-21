#pragma once

#include "stella/core/asset.hpp"

namespace stella
{
namespace core
{

  class TextureAsset : public Asset
  {
    public:
      TextureAsset (const std::string& filepath);

  };
  
}
}
