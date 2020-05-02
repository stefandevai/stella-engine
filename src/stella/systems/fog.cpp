#include "stella/systems/fog.hpp"
#include "stella/components/fog.hpp"
#include "stella/components/sprite.hpp"
#include "stella/components/camera.hpp"
#include "stella/components/position.hpp"
#include "stella/graphics/shape.hpp"
#include <chrono>

namespace stella
{
namespace system
{
  Fog::Fog (entt::registry& registry, const unsigned width, const unsigned height)
    : m_width (static_cast<float> (width)), m_height (static_cast<float> (height))
  {
    registry.on_construct<component::Fog>().connect<&Fog::initialize_fog> (this);
    registry.on_destroy<component::Fog>().connect<&Fog::remove_fog> (this);
    // std::vector<glm::vec2> vertices{
    //   glm::vec2{0.0f, 0.0f}, glm::vec2{m_width, 0.0f}, glm::vec2{m_width, m_height}, glm::vec2{0.0f, m_height}};
    // std::shared_ptr<graphics::Shape> shape = std::make_shared<graphics::Shape> (vertices, glm::vec3{0.f, 0.f, 1.f});
    // shape->set_color(2013265919);
    // m_fog_layer.Add(shape);
  }

  void Fog::update (entt::registry& registry, const double dt)
  {
    const auto camera_entity = *registry.view<stella::component::Camera>().begin();
    auto& camera_pos         = registry.get<component::Position> (camera_entity);
    m_layer.SetViewMatrix (glm::lookAt (glm::vec3 (camera_pos.x, camera_pos.y, camera_pos.z),
                                        glm::vec3 (camera_pos.x, camera_pos.y, camera_pos.z - 1.f),
                                        glm::vec3 (0.f, 1.f, 0.f)));
    m_layer.Render();
  }

  void Fog::initialize_fog (entt::registry& registry, entt::entity entity)
  {
    if (registry.has<component::Sprite> (entity))
    {
      auto& spr = registry.get<component::Sprite> (entity);
      m_layer.Add (spr.sprite);
    }
  }

  void Fog::remove_fog (entt::registry& registry, entt::entity entity)
  {
    if (registry.has<component::Sprite> (entity))
    {
      auto& spr = registry.get<component::Sprite> (entity);
      if (spr.Initialized)
      {
        m_layer.Remove (spr.sprite);
      }
    }
  }
} // namespace system
} // namespace stella
