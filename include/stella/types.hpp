#pragma once

#include "stella/graphics/font.hpp"

typedef stella::core::ResourceManager<stella::graphics::Texture, const std::string, const unsigned, const unsigned>
    TextureManager;
typedef stella::core::ResourceManager<stella::graphics::Font, const std::string, unsigned> FontManager;
