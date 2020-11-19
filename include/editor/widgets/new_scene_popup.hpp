#pragma once

#include "widget.hpp"

namespace stella
{
  class Game;
}

namespace editor
{
namespace widget
{
  struct NewScenePopup : public Widget
  {
  public:
    NewScenePopup (stella::Game& game);

    void render();

  private:
    stella::Game& m_game;
  };
} // namespace widget
} // namespace editor

