#include <stella/game2.hpp>  // IWYU pragma: export

#ifdef STELLA_BUILD_EDITOR
#include "editor/editor2.hpp"
#endif

// TEMP
#include <iostream>
// TEMP

int main (int argc, char* argv[])
{
  stella::Game game{"/Users/stefandevai/Developer/games/stella-engine/nikte2/config.json"};

#ifdef STELLA_BUILD_EDITOR
  stella::editor::Editor editor{game};
  editor.run();
#else
  game.run();
#endif

  return 0;
}

