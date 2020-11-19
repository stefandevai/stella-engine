#pragma once

#include "widget.hpp"
#include <entt/entity/entity.hpp>
#include <entt/entity/registry.hpp>

namespace stella
{
namespace core
{
  class Scene;
}
}

namespace editor
{
namespace widget
{
  struct SceneEditor : public Widget
  {
  public:
    SceneEditor();

    void reload();
    void render(std::shared_ptr<stella::core::Scene>& scene);
    void open();

  private:
    bool m_should_reload = true;
    int m_number_of_objects = 0;
  };
} // namespace widget
} // namespace editor

