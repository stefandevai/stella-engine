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
  struct LoadScenePopup : public Widget
  {
  public:
    LoadScenePopup (stella::Game& game);

    void open();
    void render();

  private:
    stella::Game& m_game;
  };
} // namespace widget
} // namespace editor

