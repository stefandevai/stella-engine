local M = {}

local Entity = require('scripts.Entity')
local Player = Entity:create_player()

local function load_assets()
  load_texture("player", "assets/sprites/player.png")
  load_texture("nikte", "assets/sprites/nikte.png")
  load_texture("tiles", "assets/sprites/tiles32x32.png")
  load_texture("tileset", "assets/sprites/tileset.png")

  load_font("1980", "assets/fonts/1980.ttf", 32)
  load_font("lato", "assets/fonts/Lato/Lato-Black.ttf", 56)
end

local function load_npc(x, y)
  local npc = Entity:create_entity()
  npc:add_component("npc", {
    script_path = "scripts/npcs/test.lua"
  })
  -- npc:add_component("name", "Test")
  npc:add_component("sprite", {
    texture = "nikte",
    layer = "tiles",
    frame_dimensions = {32, 64, 0},
  })
  npc:add_component("position", {x, y, 10})
  npc:add_component("dimension", {32, 64})
  npc:add_component("movement")
  npc:add_component("body", {
    bounding_box = {32, 32},
    bounding_box_position = {0, 32},
  })

  animation_args = {}
  animation_args["frame_dimensions"] = {32, 64}
  animation_args["animations"] = {}
  animation_args["animations"][1] = {"idle-d", {0}, 5}
  animation_args["animations"][2] = {"walking-r", {19,20,21,22,23,24,25,26}, 5}
  animation_args["animations"][3] = {"walking-u", {28,29,30,31,32,33,34,35}, 5}
  animation_args["animations"][4] = {"walking-d", {1,2,3,4,5,6,7,8}, 5}
  animation_args["animations"][5] = {"idle-l", {45}, 5}
  animation_args["animations"][6] = {"idle-r", {18}, 5}
  animation_args["animations"][7] = {"idle-u", {27}, 5}
  animation_args["animations"][8] = {"walking-l", {46,47,48,49,50,51,52,53}, 5}
  npc:add_component("animation", animation_args) 
end

local function load_player(x, y)
  Player:add_component("player")
  Player:add_component("sprite", {
    texture = "nikte",
    layer = "tiles",
    frame_dimensions = {32, 64, 0},
  })
  Player:add_component("position", {x, y, 2})
  Player:add_component("dimension", {32, 64})
  Player:add_component("movement")
  Player:add_component("body", {
    bounding_box = {32, 32},
    bounding_box_position = {0, 32},
  })

  animation_args = {}
  animation_args["frame_dimensions"] = {32, 64}
  animation_args["animations"] = {}
  animation_args["animations"][1] = {"walking-l", {46,47,48,49,50,51,52,53}, 5}
  animation_args["animations"][2] = {"walking-r", {19,20,21,22,23,24,25,26}, 5}
  animation_args["animations"][3] = {"walking-u", {28,29,30,31,32,33,34,35}, 5}
  animation_args["animations"][4] = {"walking-d", {1,2,3,4,5,6,7,8}, 5}
  animation_args["animations"][5] = {"idle-l", {45}, 5}
  animation_args["animations"][6] = {"idle-r", {18}, 5}
  animation_args["animations"][7] = {"idle-u", {27}, 5}
  animation_args["animations"][8] = {"idle-d", {0}, 5}
  Player:add_component("animation", animation_args) 
end

local function load()
  create_layer({
    name = "tiles",
    priority = 1,
    fixed = false,
  })
  create_layer({
    name = "collision",
    priority = 2,
    fixed = false,
  })
  create_layer({
    name = "foreground",
    priority = 3,
    fixed = false,
  })

  create_layer({
    name = "text",
    priority = 10,
    vert_source = "assets/shaders/sprite_batch.vert",
    frag_source = "assets/shaders/text.frag",
    fixed = false,
  })

  load_assets()
  load_player(32, 64)
  load_npc(64, 64)

  -- local test_spr = Entity:create_entity()
  -- test_spr:add_component("sprite", {
  --   texture = "nikte",
  --   layer = "tiles",
  --   frame_dimensions = {32, 64, 0},
  -- })
  -- test_spr:add_component("position", {512, 128, 3})
  -- test_spr:add_component("dimension", {32, 64})

  -- local test_text = Entity:create_entity()
  -- test_text:add_component("position", {512, 64, 0})
  -- test_text:add_component("dimension", {32, 100})
  -- test_text:add_component("text", {
  --   text = "aaaúñça",
  --   font_name = "lato",
  -- })

  -- local npc_text = Entity:create_entity()
  -- npc_text:add_component("position", {32, 128, 0})
  -- npc_text:add_component("dimension", {32, 100})
  -- npc_text:add_component("text", {
  --   text = "This will be a long long journey",
  --   font_name = "1980",
  --   color = "#ff0033ff",
  -- })
end

local camerax = 0.0
local speedx = 100.0
local frame_counter = 1
local camera_position = {0.0, 0.0}
local last_camera_x = 0.0

local function update(dt)
  local player_position = {get_position(Player.id)}
  camera_position[1] = math.min(30*32 - 896, math.max(0, player_position[1] - 896/2))
  camera_position[2] = math.min(20*32 - 504, math.max(0, player_position[2] - 504/2))
  update_camera(camera_position[1], camera_position[2], 0)
end

local function render(dt)
end

M.load = load
M.update = update
M.render = render
return M

