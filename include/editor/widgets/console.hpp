#pragma once

#include "widget.hpp"
//#include "../../lib/imgui/imgui.hpp"

namespace stella
{
namespace widget
{
  struct Console : public Widget
  {
    ImGuiTextBuffer Buf;
    ImGuiTextFilter Filter;
    ImVector<int> LineOffsets; // Index to lines offset. We maintain this with AddLog()
                               // calls, allowing us to have a random access on lines
    bool AutoScroll;
    bool ScrollToBottom;
    const ImGuiWindowFlags WindowFlags;
    ImFont*& MonoFont;

    Console (const ImGuiWindowFlags window_flags, ImFont*& mono_font);
    void clear();
    void add_log (const char* fmt, ...) IM_FMTARGS (2);
    void render ();
  };
} // namespace editor
} // namespace stella
