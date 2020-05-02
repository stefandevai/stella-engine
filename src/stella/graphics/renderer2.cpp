#include "stella/graphics/renderer2.hpp"
#include "stella/graphics/texture.hpp"
#include <vector>

namespace stella
{
namespace graphics
{
  TexPtrs RendererT::m_textures = std::vector<std::shared_ptr<graphics::Texture>>();
}
}