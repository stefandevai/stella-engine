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
  struct NewGame : public Widget
  {
  public:
    NewGame (stella::Game& game);

    void render();

  private:
    stella::Game& m_game;
  };
} // namespace widget
} // namespace editor

