#include <stella/game2.hpp>  // IWYU pragma: export

#ifdef STELLA_BUILD_EDITOR
#include "editor/editor2.hpp"
#endif

// TEMP
#include <iostream>
// TEMP

int main (int argc, char* argv[])
{
  stella::Game game{896, 504, "Nikte"};

#ifdef STELLA_BUILD_EDITOR
  stella::editor::Editor editor{game};
  editor.run();
#else
  game.run();
#endif

  return 0;
}

