local M = {}

local Entity = require('scripts.Entity')
local Map = require('scripts.ProceduralMap')

local function load_assets()
  load_texture("moon", "assets/sprites/moon_anim.png")
  load_texture("tiles", "assets/sprites/tiles.png")
  load_texture("guanaco", "assets/sprites/guanaco_frames2.png")
  load_texture("mountain", "assets/sprites/mountain3-bg.png")
end

local function load_background()
  local moon = Entity:create_entity()
  moon:add_component("sprite", {
    texture = "moon",
    layer = "background",
    frame_dimensions = {85, 85, -9},
  })
  moon:add_component("position", {678, 78, 0}) 
  moon:add_component("dimension", {85, 85}) 

  animation_args = {}
  animation_args["frame_dimensions"] = {85, 85}
  animation_args["animations"] = {}
  animation_args["animations"][1] = {"glow", {3, 0, 4, 2, 1, 4, 3, 0, 2, 4, 3}, 20}
  moon:add_component("animation", animation_args) 

  --local mountain = Entity:create_entity()
  --mountain:add_component("sprite", {
    --texture = "mountain",
    --layer = "scene"
  --})
  --mountain:add_component("position", {0, 175, -6}) 
  --mountain:add_component("dimension", {720, 230})
  --mountain:add_component("tileview")
  --mountain:add_component("scroll", {-100,0})
end

local function load_player(x, y)
  local player = Entity:create_entity()
  player:add_component("player")
  player:add_component("sprite", {
    texture = "guanaco",
    layer = "tiles",
    frame_dimensions = {80,60,0},
  })
  player:add_component("position", {x, y, 0}) 
  player:add_component("dimension", {80, 60})
  player:add_component("movement", { speed = {650, 400} })
  player:add_component("body", {
    drag = {900, 900},
    collide_with_borders = true,
  })

  animation_args = {}
  animation_args["frame_dimensions"] = {80, 60}
  animation_args["animations"] = {}
  animation_args["animations"][1] = {"idle", {5,7,6,7,5,6,5,7,9,8}, 10}
  animation_args["animations"][2] = {"run", {0,1,2,3,4}, 5}
  animation_args["animations"][3] = {"jump", {3}, 5}
  animation_args["animations"][4] = {"fall", {1}, 5}
  player:add_component("animation", animation_args) 
end

local function load()
  create_layer({
    name = "background",
    priority = 1,
    shader = "basic",
    fixed = true,
  })

  create_layer({
    name = "tiles",
    priority = 0,
    shader = "basic",
    fixed = false,
  })
  load_assets()
  load_background()
  load_player(300, 405-64-61)
  Map.load('scripts.level2.map')
end

local camerax = 0.0
local speedx = 50.0
local frame_counter = 1
local function update(dt)
  camerax = camerax + speedx*dt
  update_camera(camerax, 0.0, 0.0)
  Map.update(camerax)

  if frame_counter % 450 == 0 and speedx < 300.0 then
    speedx = speedx + 7.0
    print('Distance: ' .. camerax)
    print('Speed: ' .. speedx)
  end
  frame_counter = frame_counter + 1
end

local function render(dt)
end

M.load = load
M.update = update
M.render = render
return M

