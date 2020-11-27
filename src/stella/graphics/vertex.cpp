#include "stella/graphics/vertex.hpp"

namespace stella
{
namespace graphics
{
  bool Vertex::operator== (const Vertex& other) const { return position == other.position && texcoords == other.texcoords; }

} // namespace graphics
} // namespace stella
