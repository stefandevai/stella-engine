#pragma once

#include "widget.hpp"
#include "editor/actions.hpp"

namespace editor
{
namespace widget
{

  struct EditModeMainMenuOptions : public Widget
  {
    EditModeMainMenuOptions ();
    editor::Action render();
  };
} // namespace widget
} // namespace editor


