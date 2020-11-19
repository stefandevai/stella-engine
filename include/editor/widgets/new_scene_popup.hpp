#pragma once

#include "widget.hpp"

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
    NewScenePopup (stella::Game& game);

    bool render();

  private:
    stella::Game& m_game;
  };
} // namespace widget
} // namespace stella

