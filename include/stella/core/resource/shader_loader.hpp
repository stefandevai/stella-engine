#pragma once

#include "stella/core/resource/asset.hpp"

namespace stella
{
namespace core
{

  class ShaderLoader : public AssetLoader
  {
  public:
    ShaderLoader (const std::string& generic_filepath);
    virtual std::shared_ptr<Asset> construct();

  private:
    const std::string m_generic_filepath;
  };
  
}
}

