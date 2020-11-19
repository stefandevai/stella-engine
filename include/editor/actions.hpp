#pragma once

namespace stella
{
namespace editor
{
  enum class Action
  {
    // Global Actions
    NONE,
    QUIT_EDITOR,

    // Edit mode actions
    NEW_GAME,
    LOAD_GAME,
    SAVE_GAME,
    SAVE_GAME_AS,
    NEW_SCENE,
    LOAD_SCENE,
    SAVE_SCENE,
    SAVE_SCENE_AS,

    // Play mode actions
    QUIT_PLAY_MODE,
  };
}
}
