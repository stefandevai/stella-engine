local M = {}

local Entity = require('scripts.Entity')
local Player = Entity:create_player()

local function load_assets()
  load_texture("player", "assets/sprites/player.png")
  load_texture("tiles", "assets/sprites/tiles32x32.png")
end

local function load_player(x, y)
  Player:add_component("player")
  Player:add_component("sprite", {
    texture = "player",
    layer = "tiles",
    frame_dimensions = {32, 32, -5},
  })
  Player:add_component("position", {x, y, -5})
  Player:add_component("dimension", {32, 32})
end

local function load()
  create_layer({
    name = "tiles",
    priority = 1,
    shader = "basic",
    fixed = false,
  })

  load_assets()
  load_player(128, 32)
end

local camerax = 0.0
local speedx = 100.0
local frame_counter = 1
local camera_position = {0.0, 0.0}
local last_camera_x = 0.0

local function update(dt)
  local player_position = {get_position(Player.id)}
  camera_position[1] = math.min(59*32 - 896, math.max(0, player_position[1] - 896/2))
  camera_position[2] = math.min(29*32 - 504, math.max(0, player_position[2] - 504/2))
  update_camera(camera_position[1], camera_position[2], 0)
end

local function render(dt)
end

M.load = load
M.update = update
M.render = render
return M

