#pragma once

#include <string>

#include "./imgui/imgui.h"

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

    public:
      Console(const ImGuiWindowFlags window_flags, ImFont *&mono_font)
        : WindowFlags(window_flags), MonoFont(mono_font)
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
      }
      
      void Draw(std::string title, bool* p_open = NULL)
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
          
          ImGui::BeginChild("scrolling", ImVec2(0,0), false, ImGuiWindowFlags_HorizontalScrollbar);
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
              AddLog(editable_buffer);
              AddLog("\n");
              strcpy(editable_buffer,"");
          }

          ImGui::PopStyleVar();
          ImGui::PopStyleVar();
          
          ImGui::End();
      }

  };
}
}

