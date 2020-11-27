#pragma once

#include "widget.hpp"
#include "editor/widgets/new_system.hpp"
#include <entt/entity/entity.hpp>
#include <entt/entity/registry.hpp>

namespace stella
{
class Game;
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
    void render (std::shared_ptr<stella::Game>& game);
    void open();

  private:
    enum class SceneEditorAction
    {
      NONE,
      SELECT_SCENE,
      UPDATE_SCENE,
      RELOAD_EDITOR,
      ADD_SYSTEM,
    };

  private:
    void m_set_action (SceneEditorAction action);

  private:
    int m_number_of_objects    = 0;
    SceneEditorAction m_action = SceneEditorAction::RELOAD_EDITOR;
    NewSystem m_new_system;
  };
} // namespace widget
} // namespace editor
