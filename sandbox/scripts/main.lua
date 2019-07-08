local level1 = require('scripts.level1.main')

function load_game()
  level1.load()
end

function update_game(dt)
  level1.update(dt)
end

function render_game(dt)
  level1.render(dt)
end

function quit_game()
end

