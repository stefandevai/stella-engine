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
  struct LoadScenePopup : public Widget
  {
  public:
    LoadScenePopup (stella::Game& game);
    void open();
    bool render();

  private:
    stella::Game& m_game;
  };
} // namespace widget
} // namespace stella

