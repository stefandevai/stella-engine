#pragma once

#include <string>
#include "stella/graphics/texture2.hpp"

namespace stella::graphics
{
class TextureAtlas : public Texture
{
public:
  TextureAtlas (const std::string& filepath) : Texture (filepath, TextureType::DIFFUSE) { m_uniform = true; }
  ~TextureAtlas();

private:
  int m_horizontal_frames = 0;
  int m_vertical_frames   = 0;
  bool m_uniform          = false;
};

} // namespace stella::graphics
