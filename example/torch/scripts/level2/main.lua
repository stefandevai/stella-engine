local M = {}

local Entity = require('scripts.Entity')
local Map = require('scripts.Map')

local function load_assets()
  load_texture("moon", "assets/sprites/moon_anim.png")
  load_texture("tiles", "assets/sprites/tiles.png")
  load_texture("guanaco", "assets/sprites/guanaco-anim.png")
end

local function load_background()
  local moon = Entity:create_entity()
  moon:add_component("sprite", {
    texture = "moon",
    layer = "basic",
    frame_dimensions = {85, 85, -9},
  })
  moon:add_component("position", {478, 78, 0}) 
  moon:add_component("dimension", {85, 85}) 

  animation_args = {}
  animation_args["frame_dimensions"] = {85, 85}
  animation_args["animations"] = {}
  animation_args["animations"][1] = {"glow", {3, 0, 4, 2, 1, 4, 3, 0, 2, 4, 3}, 20}
  moon:add_component("animation", animation_args) 
end

local function load_player(x, y)
  local player = Entity:create_entity()
  player:add_component("player")
  player:add_component("sprite", {
    texture = "guanaco",
    layer = "basic",
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
  animation_args["animations"][1] = {"run", {0,1,2,3,4}, 5}
  player:add_component("animation", animation_args) 
end

local function load()
  load_assets()
  load_background()
  load_player(100, 405-64-61)
  Map.load('scripts.level2.map')
end

local function update(dt)
end

local function render(dt)
end

M.load = load
M.update = update
M.render = render
return M

