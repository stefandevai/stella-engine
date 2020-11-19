#include "editor/widgets/chat.hpp"
#include "stella/components/dimension.hpp"
#include "stella/components/player.hpp"
#include "stella/components/position.hpp"
#include "stella/components/speech_container.hpp"
#include "stella/components/text.hpp"
#include "stella/components/timer.hpp"

#include <iostream>

namespace editor
{
namespace widget
{
  Chat::Chat (ImFont*& mono_font) : Widget ("Chat"), MonoFont (mono_font)
  {
    m_open = true;
    strcpy (editable_buffer, "");
    clear();
  }

  void Chat::clear()
  {
    Buf.clear();
    LineOffsets.clear();
    LineOffsets.push_back (0);
  }

  void Chat::add_log (const char* fmt, ...)
  {
    int old_size = Buf.size();
    va_list args;
    va_start (args, fmt);
    Buf.appendfv (fmt, args);
    va_end (args);
    for (int new_size = Buf.size(); old_size < new_size; old_size++)
      if (Buf[old_size] == '\n')
        LineOffsets.push_back (old_size + 1);
    if (AutoScroll)
      ScrollToBottom = true;
  }

  void Chat::render (entt::registry& registry)
  {
    if (!ImGui::Begin (m_title_string.c_str(), &m_open, ImGuiWindowFlags_None))
    {
      ImGui::End();
      return;
    }

    ImGui::BeginChild ("scrolling-chat", ImVec2 (0, -60.f), false, ImGuiWindowFlags_HorizontalScrollbar);
    ImGui::PushStyleVar (ImGuiStyleVar_ItemSpacing, ImVec2 (0, 0));
    ImGui::PushFont (MonoFont);

    const char* buf_begin = Buf.begin();
    const char* buf_end   = Buf.end();

    ImGuiListClipper clipper;
    clipper.Begin (LineOffsets.Size);
    while (clipper.Step())
    {
      for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++)
      {
        const char* line_start = buf_begin + LineOffsets[line_no];
        const char* line_end = (line_no + 1 < LineOffsets.Size) ? (buf_begin + LineOffsets[line_no + 1] - 1) : buf_end;
        ImGui::TextUnformatted (line_start, line_end);
      }
    }
    clipper.End();

    ImGui::PopFont();
    ImGui::PopStyleVar();

    if (ScrollToBottom)
      ImGui::SetScrollHereY (1.0f);
    ScrollToBottom = false;

    ImGui::EndChild();

    ImGui::Dummy (ImVec2 (0.0f, 3.0f));
    ImGui::PushStyleColor (ImGuiCol_Separator, ImVec4 (0.43f, 0.43f, 0.5f, 0.3f));
    ImGui::Separator();
    ImGui::PopStyleColor();
    ImGui::Dummy (ImVec2 (0.0f, 6.0f));

    // Persistent focus
    // ImGui::SetKeyboardFocusHere();
    ImGui::PushStyleVar (ImGuiStyleVar_FrameRounding, 2);
    ImGui::PushStyleVar (ImGuiStyleVar_FramePadding, ImVec2 (12.0f, 6.0f));

    ImGui::PushID ("chat-log");
    if (ImGui::InputText ("", editable_buffer, IM_ARRAYSIZE (editable_buffer), ImGuiInputTextFlags_EnterReturnsTrue))
    {
      if (strlen (editable_buffer) > 0)
      {
        add_log ("%s", editable_buffer);
        add_log ("%c", '\n');

        const auto player_entity = *registry.view<stella::component::Player>().begin();

        auto text_entity = registry.create();
        registry.emplace<stella::component::Position> (text_entity, 0.0f, -100.0f);
        registry.emplace<stella::component::Text> (
            text_entity, m_converter.from_bytes (std::string (editable_buffer)), "1980");
        registry.emplace<stella::component::Timer> (
            text_entity,
            3000.0,
            0.0,
            stella::component::Timer::Type::DECREASE,
            [] (entt::registry& r, const entt::entity e) { r.destroy (e); },
            true);

        auto& container = registry.get_or_emplace<stella::component::SpeechContainer> (player_entity);
        container.messages.push_back (text_entity);

        // Clears the text buffer
        strcpy (editable_buffer, "");
      }
    }
    ImGui::PopID();

    ImGui::PopStyleVar();
    ImGui::PopStyleVar();

    ImGui::End();
  }
} // namespace widget
} // namespace editor
