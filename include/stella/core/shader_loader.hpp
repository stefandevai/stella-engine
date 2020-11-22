#pragma once

#include "stella/core/asset.hpp"

namespace stella
{
namespace core
{

  class ShaderLoader : public AssetLoader
  {
  public:
    ShaderLoader (const std::string& vertex_filepath, const std::string& fragment_filepath);
    virtual std::shared_ptr<Asset> construct();

  private:
    const std::string m_vertex_filepath;
    const std::string m_fragment_filepath;
  };
  
}
}

