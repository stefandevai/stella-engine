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
    SHADER,
    MODEL,
  };

  class Asset
  {
  public:
    virtual ~Asset() {}
  };

  class AssetLoader
  {
  public:
    virtual ~AssetLoader() {}

    virtual std::shared_ptr<Asset> construct() = 0;

  protected:
    const AssetType m_type;

  protected:
    AssetLoader (const AssetType type) : m_type (type) {}
  };

}
}
