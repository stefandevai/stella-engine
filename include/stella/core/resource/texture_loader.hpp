#pragma once

#include "stella/core/resource/asset.hpp"
#include "stella/graphics/texture2.hpp"

namespace stella
{
namespace core
{
  enum class TextureMode
  {
    NONE,
    SINGLE,
    UNIFORM_ATLAS,
  };

  class TextureLoader : public AssetLoader
  {
  public:
    // Load single texture
    TextureLoader (const std::string& filepath, const graphics::TextureType type);
    // Load uniform texture atlas
    TextureLoader (const std::string& filepath, const graphics::TextureType type, const int horizontal_frames, const int vertical_frames);

    virtual std::shared_ptr<Asset> construct();

  private:
    const std::string m_filepath;
    const graphics::TextureType m_type;
    const TextureMode m_mode;
    const int m_horizontal_frames;
    const int m_vertical_frames;
  };

} // namespace core
} // namespace stella
