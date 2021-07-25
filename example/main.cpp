#include <stella/game2.hpp> // IWYU pragma: export

#ifdef STELLA_BUILD_EDITOR
  #include "editor/editor2.hpp"
#endif

int main (int argc, char* argv[])
{
  /* const std::string game_dir = "/Users/stefandevai/Developer/games/stella-engine/example"; */
  const std::string game_dir = "./";

#ifdef STELLA_BUILD_EDITOR
  editor::Editor editor{game_dir};
  editor.run();
#else
  stella::Game game{game_dir};
  game.run();
#endif

  return 0;
}
