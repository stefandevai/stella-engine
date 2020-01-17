#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H 
#include <vector>
#include <map>
#include "stella/graphics/opengl.h"
#include <glm/glm.hpp>

#include "./layer.h"
#include "../shader.h"
#include "stella/graphics/texture.h"
#include "stella/graphics/font.h"

namespace stella {
namespace graphics {

class TextLayer : public Layer {
  public:
    TextLayer(float width, float height, bool fixed = true);
    ~TextLayer();
    void Render() override;
  private:
    Font m_font{"assets/fonts/1980.ttf"};
};
} // namespace graphics
} // namespace stella