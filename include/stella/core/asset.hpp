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
  public:
    Asset (const AssetType type, const std::string& filepath) : m_type (type), m_filepath (filepath) {};

  protected:
    AssetType m_type = AssetType::NONE;
    std::string m_filepath;
  };

}
}
