local level2 = require('scripts.level2.main')

function load_game()
  level2.load()
end

function update_game(dt)
  level2.update(dt)
end

function render_game(dt)
  level2.render(dt)
end

function quit_game()
end

