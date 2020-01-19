#pragma once

#include <string>
#include <iostream>
#include <entt/entity/registry.hpp>

#include "./imgui/imgui.h"
#include "stella/components/text_component.h"
#include "stella/components/position_component.h"
#include "stella/components/dimension_component.h"
#include "stella/components/timer_component.h"

namespace stella
{
namespace editor
{
  class Console
  {
    private:
      const ImGuiWindowFlags WindowFlags;
      bool AutoScroll = true;
      bool ScrollToBottom = false;
      ImVector<int> LineOffsets;
      ImFont *&MonoFont;
      ImGuiTextBuffer Buf;
      char editable_buffer[512];
      entt::registry &m_registry;

    public:
      Console(const ImGuiWindowFlags window_flags, ImFont *&mono_font, entt::registry &registry)
        : WindowFlags(window_flags), MonoFont(mono_font), m_registry(registry)
      {
          Clear();
      }
      ~Console() {}

      void Clear()
      {
          Buf.clear();
          LineOffsets.clear();
          LineOffsets.push_back(0);
      }

      void AddLog(const char* fmt, ...) IM_FMTARGS(2)
      {
          int old_size = Buf.size();
          va_list args;
          va_start(args, fmt);
          Buf.appendfv(fmt, args);
          va_end(args);
          for (int new_size = Buf.size(); old_size < new_size; old_size++)
              if (Buf[old_size] == '\n')
                  LineOffsets.push_back(old_size + 1);
          if (AutoScroll)
              ScrollToBottom = true;
      }
      
      void Draw(std::string title, entt::registry &registry, bool* p_open = NULL)
      {
          if (!ImGui::Begin(title.c_str(), p_open, WindowFlags))
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
        //   ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(0.43f, 0.43f, 0.5f, 0.3f));
        //   ImGui::Separator();
        //   ImGui::PopStyleColor();
        //   ImGui::Dummy(ImVec2(0.0f, 6.0f));
          
        //   if (clear)
        //   {
        //       Clear();
        //   }
          
          ImGui::BeginChild("scrolling", ImVec2(0,-60.f), false, ImGuiWindowFlags_HorizontalScrollbar);
          ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
          ImGui::PushFont(MonoFont);

          const char* buf_begin = Buf.begin();
          const char* buf_end = Buf.end();

          ImGuiListClipper clipper;
              clipper.Begin(LineOffsets.Size);
              while (clipper.Step())
              {
                  for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++)
                  {
                      const char* line_start = buf_begin + LineOffsets[line_no];
                      const char* line_end = (line_no + 1 < LineOffsets.Size) ? (buf_begin + LineOffsets[line_no + 1] - 1) : buf_end;
                      ImGui::TextUnformatted(line_start, line_end);
                  }
              }
          clipper.End();

          ImGui::PopFont();
          ImGui::PopStyleVar();

          if (ScrollToBottom)
              ImGui::SetScrollHereY(1.0f);
          ScrollToBottom = false;

          ImGui::EndChild();

          ImGui::Dummy(ImVec2(0.0f, 3.0f));
          ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(0.43f, 0.43f, 0.5f, 0.3f));
          ImGui::Separator();
          ImGui::PopStyleColor();
          ImGui::Dummy(ImVec2(0.0f, 6.0f));

          ImGui::SetKeyboardFocusHere();
          ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 2);
          ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(12.0f, 6.0f));
          
          if(ImGui::InputText("", editable_buffer, IM_ARRAYSIZE(editable_buffer), ImGuiInputTextFlags_EnterReturnsTrue))
          {
              if (strlen(editable_buffer) > 0)
              {
                AddLog(editable_buffer);
                AddLog("\n");
                auto text_entity = registry.create();
                registry.assign<stella::components::PositionComponent>(text_entity, 350.f, 268.f);
                registry.assign<stella::components::DimensionComponent>(text_entity, 100.f, 100.f);
                registry.assign<stella::components::TextComponent>(text_entity, std::string(editable_buffer), "1980");
                registry.assign<stella::components::TimerComponent>(text_entity, components::TimerComponent::TimerEvent::Destroy, 3000);
                // Clears the text buffer
                strcpy(editable_buffer, "");
              }
          }

          ImGui::PopStyleVar();
          ImGui::PopStyleVar();
          
          ImGui::End();
      }

  };
}
}
