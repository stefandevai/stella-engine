#pragma once

#include "../../lib/imgui/imgui.h"
#include "editor/state.h"

#define ICON_FA_EDIT          u8"\uf044"
#define ICON_FA_PLAY          u8"\uf04b"
#define ICON_FA_MOUSE_POINTER u8"\uf245"

namespace stella
{
namespace editor
{
  class Toolbar
  {
  public:
    Toolbar();
    ~Toolbar();
    void render (editor::State& state, editor::Tool& tool);
  };
} // namespace editor
} // namespace stella
