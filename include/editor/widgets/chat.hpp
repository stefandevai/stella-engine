#pragma once

#include <entt/entity/registry.hpp> // IWYU pragma: export
#include <string>

#include <codecvt>
#include <locale>

#include "widget.hpp"
//#include "../../lib/imgui/imgui.hpp"

namespace stella
{
namespace widget
{
  class Chat : public Widget
  {
  private:
    const ImGuiWindowFlags WindowFlags;
    bool AutoScroll     = true;
    bool ScrollToBottom = false;
    ImVector<int> LineOffsets;
    ImFont*& MonoFont;
    ImGuiTextBuffer Buf;
    char editable_buffer[512];
    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> m_converter;

  public:
    Chat (const ImGuiWindowFlags window_flags, ImFont*& mono_font);
    void clear();
    void add_log (const char* fmt, ...) IM_FMTARGS (2);
    void render (entt::registry& registry);
  };
} // namespace widget
} // namespace stella
