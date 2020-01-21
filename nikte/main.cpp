#include "game.h"

#ifdef STELLA_BUILD_EDITOR
#include "editor/editor_gui.h"
#endif

int main(int argc, char *argv[]) {
  nikte::Game game;

#ifdef STELLA_BUILD_EDITOR
  stella::editor::EditorGui editor{game};
#endif

  game.run();
  return 0;
}