#include "stella/graphics/debug_gui.h"

DebugGui::DebugGui() {
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;
}

DebugGui::~DebugGui() {
  ImGui_ImplSdlGL3_Shutdown();
  ImGui::DestroyContext();
}

void DebugGui::Init(SDL_Window *window) {
  this->Window = window;
  ImGui_ImplSdlGL3_Init(this->Window);
  ImGui::StyleColorsDark();
}

void DebugGui::Update() {
  ImGui_ImplSdlGL3_NewFrame(this->Window);

  bool show_another_window = true;
  ImGui::Begin("Another Window", &show_another_window);
  static float f = 0.0f;
  static int counter = 0;
  ImGui::Text("Hello, world!");                           // Display some text (you can use a format string too)
  ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
  ImGui::Text("Hello from another window!");
  ImGui::End();

  ImGui::Render();
  ImGui_ImplSdlGL3_RenderDrawData(ImGui::GetDrawData());
}

void DebugGui::GetInput(SDL_Event& e) {
  ImGui_ImplSdlGL3_ProcessEvent(&e);
}

