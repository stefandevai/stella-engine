#include "stella/systems/fog.hpp"
#include "stella/components/fog.hpp"
#include "stella/graphics/shape.hpp"

namespace stella
{
namespace system
{
  
    Fog::Fog (entt::registry& registry, const unsigned width, const unsigned height)
      : m_width(static_cast<float>(width)), m_height(static_cast<float>(height))
    {
      registry.on_construct<component::Fog>().connect<&Fog::initialize_fog> (this);
      std::vector<glm::vec2> vertices{
        glm::vec2{0.0f, 0.0f}, glm::vec2{m_width, 0.0f}, glm::vec2{m_width, m_height}, glm::vec2{0.0f, m_height}};
      std::shared_ptr<graphics::Shape> shape = std::make_shared<graphics::Shape> (vertices, glm::vec3{0.f, 0.f, 1.f});
      shape->set_color(2013265919);
      m_fog_layer.Add(shape);
    }

    void Fog::update (entt::registry& registry, const double dt)
    {
        m_fog_layer.Render();
    }

    void Fog::initialize_fog (entt::registry& registry, entt::entity entity)
    {
    }
} // namespace system
} // namespace stella
