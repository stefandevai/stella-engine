#include "stella/core/resource/font_loader.hpp"
#include "stella/graphics/font.hpp"

namespace stella::core
{
FontLoader::FontLoader (const std::string& filepath, const std::size_t size) : AssetLoader (AssetType::FONT), m_filepath (filepath), m_size (size) {}

std::shared_ptr<Asset> FontLoader::construct() { return std::make_shared<graphics::Font> (m_filepath, m_size); }

} // namespace stella::core
