#include "game.h"

#ifdef STELLA_BUILD_EDITOR
  #include "editor/editor.h"
#endif

int main (int argc, char* argv[])
{
  nikte::Game game;

#ifdef STELLA_BUILD_EDITOR
  stella::editor::Editor editor{game};
  editor.run();
#else
  game.run();
#endif

  return 0;
}