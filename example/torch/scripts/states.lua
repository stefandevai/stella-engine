game_states = {"GAME_LOADING", "GAME_LOADED", "GAME_NOT_LOADED", "MAIN_MENU", "GAME_LOOP", "GAME_PAUSED", "GAME_OVER"}
-- GAME_LOADING
initial_state = 1
current_state = initial_state
load_fail = false

function load ()
  print('Loading Game...')
  if load_fail == true then
    -- GAME_NOT_LOADED
    current_state = 3
  else
    if update_game ~= nil then
      -- GAME_LOOP
      current_state = 5
    end
  end
end

function after_load()
  print('Game loaded')
end

function loop()
  update_game()
end

function game_main_menu ()
  print('game menu')
end

function game_loop ()
  print('game loop')
end

function game_paused ()
  current_state = 6
  print('game paused')
end

function game_over ()
  current_state = 7
  print('game over')
end
