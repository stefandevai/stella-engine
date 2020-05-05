local M = {}

local Entity = require('scripts.Entity')
local Player = Entity:create_player()
local flowers = require('scripts.demo.flowers')

local function load_assets()
  load_texture("nikte", "assets/sprites/nikte.png")
  load_texture("tileset", "assets/sprites/tilesetv2.png")

  load_font("1980", "assets/fonts/1980.ttf", 32)
  load_font("lato", "assets/fonts/Lato/Lato-Black.ttf", 56)
end

local function load_npc(x, y)
  local npc = Entity:create_entity()
  -- npc:add_component("name", "Test")
  npc:add_component("position", {x, y, 1})
  npc:add_component("dimension", {32, 64})
  npc:add_component("sprite", {
    texture = "nikte",
    layer = "collision",
    frame_dimensions = {9, 6},
  })
  npc:add_component("movement")
  npc:add_component("body", {
    movement_speed = 3,
    bounding_box = {32, 32},
    bounding_box_position = {0, 32},
  })
  npc:add_component("npc", {
    script_path = "scripts/npcs/test.lua"
  })

  animation_args = {}
  animation_args["loop"] = true
  animation_args["animations"] = {}
  animation_args["animations"][1] = {"idle-b", {0}, 0.1}
  animation_args["animations"][2] = {"walking-r", {19,20,21,22,23,24,25,26}, 0.1}
  animation_args["animations"][3] = {"walking-t", {28,29,30,31,32,33,34,35}, 0.1}
  animation_args["animations"][4] = {"walking-b", {1,2,3,4,5,6,7,8}, 0.1}
  animation_args["animations"][5] = {"idle-l", {45}, 0.1}
  animation_args["animations"][6] = {"idle-r", {18}, 0.1}
  animation_args["animations"][7] = {"idle-t", {27}, 0.1}
  animation_args["animations"][8] = {"walking-l", {46,47,48,49,50,51,52,53}, 0.1}
  npc:add_component("animation", animation_args) 
  npc:add_component("character_animation")
end

local function load_player(x, y)
  Player:add_component("player")
  Player:add_component("position", {x, y, 9})
  Player:add_component("dimension", {32, 64})
  Player:add_component("sprite", {
    texture = "nikte",
    layer = "collision",
    frame_dimensions = {9, 6},
  })
  Player:add_component("movement")
  Player:add_component("body", {
    movement_speed = 10,
    bounding_box = {32, 32},
    bounding_box_position = {0, 32},
  })

  animation_args = {}
  animation_args["loop"] = true
  animation_args["animations"] = {}
  animation_args["animations"][1] = {"walking-l", {46,47,48,49,50,51,52,53}, 0.1}
  animation_args["animations"][2] = {"walking-r", {19,20,21,22,23,24,25,26}, 0.1}
  animation_args["animations"][3] = {"walking-t", {28,29,30,31,32,33,34,35}, 0.1}
  animation_args["animations"][4] = {"walking-b", {1,2,3,4,5,6,7,8}, 0.1}
  animation_args["animations"][5] = {"idle-l", {45}, 0.1}
  animation_args["animations"][6] = {"idle-r", {18}, 0.1}
  animation_args["animations"][7] = {"idle-t", {27}, 0.1}
  animation_args["animations"][8] = {"idle-b", {0}, 0.1}
  Player:add_component("animation", animation_args)
  Player:add_component("character_animation")
end

local function load()
  create_layer({
    name = "tiles",
    priority = 1,
    fixed = false,
    vert_source = "assets/shaders/sprite_batch.vert",
    frag_source = "assets/shaders/sprite_batch.frag",
  })
  create_layer({
    name = "collision",
    priority = 2,
    fixed = false,
    vert_source = "assets/shaders/sprite_batch.vert",
    frag_source = "assets/shaders/sprite_batch.frag",
  })
  create_layer({
    name = "foreground",
    priority = 3,
    fixed = false,
    vert_source = "assets/shaders/sprite_batch.vert",
    frag_source = "assets/shaders/sprite_batch.frag",
  })
  create_layer({
    name = "shapes",
    priority = 4,
    fixed = true,
    vert_source = "assets/shaders/shape.vert",
    frag_source = "assets/shaders/shape.frag",
    shader = "shape",
  })

  -- create_layer({
  --   name = "fog",
  --   priority = 4,
  --   fixed = true,
  --   shader = "shape",
  --   vert_source = "assets/shaders/fog.vert",
  --   frag_source = "assets/shaders/fog.frag",
  -- })

  create_layer({
    name = "text",
    priority = 10,
    vert_source = "assets/shaders/sprite_batch.vert",
    frag_source = "assets/shaders/text.frag",
    fixed = false,
  })

  -- load_assets()
  load_player(480, 512)
  load_npc(512, 512)
  --flowers.load()

  -- local test_shape = Entity:create_entity()
  -- test_shape:add_component("position", {0, 10, 5})
  -- test_shape:add_component("shape", {
  --   vertices = {{32.0,32.0,0.0},{864.0,32.0,0.0},{864.0,472.0,0.0},{32.0,472.0,0.0}},
  --   layer = "shapes"
  -- })

  -- test_shape:add_component("color", {
  --   --rgba = {255, 255, 255, 100},
  --   hex = "#3769ab44",
  -- })

  -- test_shape:add_component("transform", {
  --   rotation = {1,0,0},
  --   scale = {1,1,1},
  -- })

  -- local test_spr = Entity:create_entity()
  -- test_spr:add_component("position", {410, 224, 9})
  -- test_spr:add_component("dimension", {32, 64})
  -- test_spr:add_component("sprite", {
  --   texture = "nikte",
  --   layer = "collision",
  --   frame_dimensions = {9, 6},
  -- })

  -- local test_spr2 = Entity:create_entity()
  -- test_spr2:add_component("position", {400, 200, 10})
  -- test_spr2:add_component("dimension", {32, 64})
  -- test_spr2:add_component("sprite", {
  --   texture = "nikte",
  --   layer = "tiles",
  --   frame_dimensions = {9, 6},
  -- })


  -- local test_spr = Entity:create_entity()
  -- test_spr:add_component("sprite", {
  --   texture = "nikte",
  --   layer = "tiles",
  --   frame_dimensions = {32, 64, 0},
  -- })
  -- test_spr:add_component("position", {512, 128, 3})
  -- test_spr:add_component("dimension", {32, 64})

  --local test_text = Entity:create_entity()
  --test_text:add_component("position", {512, 64, 0})
  --test_text:add_component("dimension", {32, 100})
  --test_text:add_component("text", {
    --text = "aaaúñça",
    --font_name = "lato",
    --color = "#4db88fff",
  --})

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
  -- local player_position = {get_position(Player.id)}
  -- camera_position[1] = math.min(e_map_width*32 - e_screen_width, math.max(0, player_position[1] - e_screen_width/2))
  -- camera_position[2] = math.min(e_map_height*32 - e_screen_height, math.max(0, player_position[2] - e_screen_height/2))
  -- update_camera(camera_position[1], camera_position[2], 0)
  -- -- flowers.update()
end

local function render(dt)
end

M.load_assets = load_assets
M.load = load
M.update = update
M.render = render
return M

