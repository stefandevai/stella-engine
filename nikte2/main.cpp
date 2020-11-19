#include <stella/game2.hpp>  // IWYU pragma: export
#include <spdlog/spdlog.h>

#ifdef STELLA_BUILD_EDITOR
#include "editor/editor2.hpp"
#endif

int main (int argc, char* argv[])
{
  if constexpr (STELLA_BUILD_EDITOR)
  {
    spdlog::set_level(spdlog::level::debug);
  }

  stella::Game game{"/Users/stefandevai/Developer/games/stella-engine/nikte2/config.json"};

  if constexpr (STELLA_BUILD_EDITOR)
  {
    editor::Editor editor{game};
    editor.run();
  }
  else
  {
    game.run();
  }

  return 0;
}

