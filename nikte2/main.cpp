#include <stella/game2.hpp>  // IWYU pragma: export

#ifdef STELLA_BUILD_EDITOR
#include "editor/editor2.hpp"
#endif

int main (int argc, char* argv[])
{
  const std::string game_filepath = "/Users/stefandevai/Developer/games/stella-engine/nikte2/config.json";

  //if constexpr (STELLA_BUILD_EDITOR)
  //{
  editor::Editor editor{game_filepath};
  editor.run();
  //}
  //else
  //{
    //stella::Game game{game_filepath};
    //game.run();
  //}

  return 0;
}

