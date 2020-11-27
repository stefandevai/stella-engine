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
    NewScenePopup (std::shared_ptr<stella::Game>& game);

    void render();

  private:
    std::shared_ptr<stella::Game>& m_game;
  };
} // namespace widget
} // namespace editor
