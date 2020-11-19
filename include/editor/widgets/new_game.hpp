#pragma once

#include "widget.hpp"

namespace editor
{
namespace widget
{
  typedef std::function<void(const std::string&, const std::string&, const int, const int)> NewGameFunction;

  struct NewGame : public Widget
  {
  public:
    NewGame ();

    void render(NewGameFunction create_new_game);
  };
} // namespace widget
} // namespace editor

