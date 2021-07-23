#include "game.hpp"

#ifdef STELLA_BUILD_EDITOR
  #include "editor/editor.hpp"
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