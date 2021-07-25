#include "stella/graphics/renderer.hpp"
#include "stella/graphics/texture.hpp"
#include <vector>

namespace stella
{
namespace graphics
{
  TexPtrs RendererT::m_textures = std::vector<std::shared_ptr<graphics::Texture>>();
}
} // namespace stella
