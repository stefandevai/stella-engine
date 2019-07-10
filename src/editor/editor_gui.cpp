#include "editor/editor_gui.h"

#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>

namespace stella
{
namespace editor
{

  EditorGui::EditorGui()
  {

  }

  EditorGui::~EditorGui()
  {
  }

  void EditorGui::init(SDL_Window *window, SDL_GLContext gl_context, const char *glsl_version)
  {
    m_window = window;
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplSDL2_InitForOpenGL(m_window, gl_context);
    ImGui_ImplOpenGL3_Init(glsl_version);
  }

  void EditorGui::configure_input(SDL_Event &event)
  {
    ImGui_ImplSDL2_ProcessEvent(&event);
  }

  void EditorGui::render()
  {
    if (m_window != nullptr)
    {
      //ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
      ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoMove;

      ImGui_ImplOpenGL3_NewFrame();
      ImGui_ImplSDL2_NewFrame(m_window);
      ImGui::NewFrame();
      {
        static int counter = 0;

        ImGui::SetNextWindowSize(ImVec2(200,200), ImGuiCond_Once);
        ImGui::SetNextWindowPos(ImVec2(0,21), ImGuiCond_Once);
        ImGui::Begin("Hello, world!", nullptr, window_flags);                          // Create a window called "Hello, world!" and append into it.

        {
            if (ImGui::BeginMainMenuBar())
            {
              if (ImGui::BeginMenu("File"))
              {
                if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
                if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
                ImGui::Separator();
                if (ImGui::MenuItem("Cut", "CTRL+X")) {}
                if (ImGui::MenuItem("Copy", "CTRL+C")) {}
                if (ImGui::MenuItem("Paste", "CTRL+V")) {}
                ImGui::EndMenu();
              }
            if (ImGui::BeginMenu("Edit"))
            {
                if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
                if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
                ImGui::Separator();
                if (ImGui::MenuItem("Cut", "CTRL+X")) {}
                if (ImGui::MenuItem("Copy", "CTRL+C")) {}
                if (ImGui::MenuItem("Paste", "CTRL+V")) {}
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
          }
        }

        ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)

        if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
      }
      ImGui::Render();
      ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
  }

  void EditorGui::clean()
  {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
  }

}
}

