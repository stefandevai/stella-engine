#include <stella/game2.hpp>  // IWYU pragma: export
#include <spdlog/spdlog.h>

#ifdef STELLA_BUILD_EDITOR
#include "editor/editor2.hpp"
#endif

int main (int argc, char* argv[])
{
  stella::Game game{"/Users/stefandevai/Developer/games/stella-engine/nikte2/config.json"};

#ifdef STELLA_BUILD_EDITOR
  spdlog::set_level(spdlog::level::debug);
  editor::Editor editor{game};
  editor.run();
#else
  spdlog::set_level(spdlog::level::warn);
  game.run();
#endif

  return 0;
}

