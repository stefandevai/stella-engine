#include "game.h"

#ifdef STELLA_BUILD_EDITOR
#include "editor/editor_gui.h"
#endif

int main(int argc, char *argv[]) {
  nikte::Game game;

#ifdef STELLA_BUILD_EDITOR
  stella::editor::EditorGui editor{game};
  editor.run();
#else
  game.run();
#endif
  
  return 0;
}