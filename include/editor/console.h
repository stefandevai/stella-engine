#pragma once

#include <entt/entity/registry.hpp>
#include <iostream>
#include <string>

#include <codecvt>
#include <locale>

#include "../../lib/imgui/imgui.h"
#include "stella/components/dimension.h"
#include "stella/components/player.h"
#include "stella/components/position.h"
#include "stella/components/speech_container.h"
#include "stella/components/text.h"
#include "stella/components/timer.h"

namespace stella
{
namespace editor
{
  class Console
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
    Console (const ImGuiWindowFlags window_flags, ImFont*& mono_font) : WindowFlags (window_flags), MonoFont (mono_font)
    {
      strcpy (editable_buffer, "");
      Clear();
    }
    ~Console() {}

    void Clear()
    {
      Buf.clear();
      LineOffsets.clear();
      LineOffsets.push_back (0);
    }

    void AddLog (const char* fmt, ...) IM_FMTARGS (2)
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

    void Draw (std::string title, entt::registry& registry, bool* p_open = NULL)
    {
      if (!ImGui::Begin (title.c_str(), p_open, ImGuiWindowFlags_None))
      {
        ImGui::End();
        return;
      }

      //   ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 2);
      //   ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(12.0f, 6.0f));
      //   bool clear = ImGui::Button("Clear");
      //   ImGui::PopStyleVar();
      //   ImGui::PopStyleVar();

      //   ImGui::Dummy(ImVec2(0.0f, 3.0f));
      //   ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(0.43f, 0.43f, 0.5f,
      //   0.3f)); ImGui::Separator(); ImGui::PopStyleColor();
      //   ImGui::Dummy(ImVec2(0.0f, 6.0f));

      //   if (clear)
      //   {
      //       Clear();
      //   }

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
          const char* line_end =
              (line_no + 1 < LineOffsets.Size) ? (buf_begin + LineOffsets[line_no + 1] - 1) : buf_end;
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
          AddLog ("%s", editable_buffer);
          AddLog ("%c", '\n');

          const auto player_entity = *registry.view<stella::component::Player>().begin();
          auto& player_pos         = registry.get<component::Position> (player_entity);
          auto& player_dim         = registry.get<component::Dimension> (player_entity);

          auto text_entity = registry.create();
          registry.assign<stella::component::Position> (
              text_entity, player_pos.x + player_dim.w / 2.f, player_pos.y - 4.f);
          registry.assign<stella::component::Text> (
              text_entity, m_converter.from_bytes (std::string (editable_buffer)), "1980");
          // registry.assign<stella::component::Timer> (text_entity, component::Timer::TimerEvent::Destroy, 3000);
          registry.assign<component::Timer> (
              text_entity,
              3000.0,
              0.0,
              component::Timer::Type::DECREASE,
              [] (entt::registry& r, const entt::entity e) { r.destroy (e); },
              true);

          auto& container = registry.get_or_assign<component::SpeechContainer> (player_entity);
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
  };
} // namespace editor
} // namespace stella
