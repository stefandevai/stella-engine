#include "stella/graphics/layers/layer.hpp"

#include <iostream>
#include <algorithm>

namespace stella
{
namespace graphics
{
    LayerT::LayerT(entt::registry& registry, const bool fixed)
    : m_registry(registry), fixed(fixed)
    {

    }
}
}