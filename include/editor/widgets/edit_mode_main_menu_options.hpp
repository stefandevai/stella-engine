#pragma once

#include "widget.hpp"
#include "editor/actions.hpp"

namespace stella
{
namespace widget
{

  struct EditModeMainMenuOptions : public Widget
  {
    EditModeMainMenuOptions ();
    editor::Action render();
  };
} // namespace widget
} // namespace stella


