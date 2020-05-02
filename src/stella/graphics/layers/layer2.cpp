#include "stella/graphics/layers/layer2.hpp"

#include <iostream>
#include <algorithm>

namespace stella
{
namespace graphics
{
    LayerT::LayerT()
    {

    }

    LayerT::~LayerT()
    {
        m_entities.clear();
    }

    void LayerT::add (entt::entity entity) { m_entities.insert (entity); }

    void LayerT::remove (entt::entity entity)
    {
        auto it = std::find (m_entities.begin(), m_entities.end(), entity);
        if (it != m_entities.end())
        {
            m_entities.erase (it);
        }
        else
        {
            std::cout << "This entity is not on the layer." << '\n';
        }
    }
}
}