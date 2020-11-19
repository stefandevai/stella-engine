#include <stella/game2.hpp>  // IWYU pragma: export
#include <spdlog/spdlog.h>

#ifdef STELLA_BUILD_EDITOR
#include "editor/editor2.hpp"
#endif

int main (int argc, char* argv[])
{
  const std::string game_filepath = "/Users/stefandevai/Developer/games/stella-engine/nikte2/config.json";

  if constexpr (STELLA_BUILD_EDITOR)
  {
    spdlog::set_level(spdlog::level::debug);
    editor::Editor editor;
    editor.run();
  }
  else
  {
    stella::Game game{game_filepath};
    game.run();
  }

  return 0;
}

