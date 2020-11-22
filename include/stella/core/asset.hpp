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

  struct AssetData
  {
  public:
    const std::string filepath;
    const AssetType type;

  public:
    virtual ~AssetData() {}

    virtual std::shared_ptr<Asset> construct() = 0;

  protected:
    AssetData (const std::string& filepath, const AssetType type) : filepath (filepath), type (type) {}
  };

}
}
