local demo = require('scripts.demo.main')

function load_game()
  demo.load()
end

function update_game(dt)
  demo.update(dt)
end

function render_game(dt)
  demo.render(dt)
end

function quit_game()
end

