#include "stella/graphics/camera.hpp"
#include <entt/entt.hpp>

namespace stella
{
namespace graphics
{

  Camera::Camera () { m_target = entt::null; }

  void Camera::update (entt::registry& registry)
  {
    if (m_target == entt::null)
    {
      return;
    }
  }

  void Camera::move (const float x, const float y, const float z)
  {
    m_position.x += x;
    m_position.y += y;
    m_position.z += z;

    m_front.x += x;
    m_front.y += y;
    m_front.z += z;
  }

  void Camera::set_position (const float x, const float y, const float z)
  {
    m_position.x = x;
    m_position.y = y;
    m_position.z = z;

    m_front.x = x;
    m_front.y = y;
    m_front.z = z - 501;
  }

  void Camera::set_target (const entt::entity& target)
  {
    m_target = target;
  }

  void Camera::remove_target ()
  {
    m_target = entt::null;
  }

}
}


