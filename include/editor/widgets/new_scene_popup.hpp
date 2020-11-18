#pragma once

#include "widget.hpp"
#include <entt/entity/entity.hpp>
#include <entt/entity/registry.hpp>

namespace stella
{
  class Game;
}

namespace stella
{
namespace widget
{
  struct NewScenePopup : public Widget
  {
  public:
    NewScenePopup(stella::Game& game);

    bool render();
    void open();

  private:
    stella::Game& m_game;
  };
} // namespace widget
} // namespace stella

