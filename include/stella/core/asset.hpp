#pragma once

#include <string>

namespace stella
{
namespace core
{

  enum class AssetType
  {
    NONE,
    TEXTURE,
    SOUND,
    MODEL,
  };

  class Asset
  {
  };

  struct AssetLoader
  {
  public:
    const std::string filepath;
    const AssetType type;

  public:
    virtual ~AssetLoader() {}

    virtual std::shared_ptr<Asset> construct() = 0;

  protected:
    AssetLoader (const std::string& filepath, const AssetType type) : filepath (filepath), type (type) {}
  };

}
}
