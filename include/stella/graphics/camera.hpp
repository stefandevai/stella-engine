#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <entt/fwd.hpp>

namespace stella
{
namespace graphics
{
  enum class CameraState
  {
    IDLE,
    FOLLOW,
  };

  class Camera
  {
  public:
    Camera();

    void move (const float x, const float y, const float z);
    void update (entt::registry& registry);
    inline const glm::vec3& get_position() const { return m_position; }
    inline glm::mat4 get_view_matrix() const { return glm::lookAt(m_position, m_front, m_up); }
    inline const glm::vec3& get_saved_position () const { return m_saved_position; }
    void set_position (const float x, const float y, const float z);
    void set_target (const entt::entity& target);
    void remove_target ();
    inline void save_position () { m_saved_position = m_position; }

  private:
    glm::vec3 m_position{0.0f, 0.0f, 500.0f};
    glm::vec3 m_front{0.0f, 0.0f, -1.0f};
    const glm::vec3 m_up{0.0f, 1.0f, 0.0f};
    glm::vec3 m_saved_position{0.0f};
    entt::entity m_target;
    //CameraState m_state = CameraState::IDLE;
  };

}
}

