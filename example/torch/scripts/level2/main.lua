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
  moon:add_component("sprite", {"moon", "basic", {85, 85, -9}})
  moon:add_component("position", {478, 78}) 
  moon:add_component("dimension", {85, 85}) 

  animation_args = {}
  animation_args["frame_dimensions"] = {85, 85}
  animation_args["animations"] = {}
  animation_args["animations"][1] = {"glow", {3, 0, 4, 2, 1, 4, 3, 0, 2, 4, 3}, 20}
  moon:add_component("animation", animation_args) 
end

local function load_blocks()
  local blocks = {}
  local over_blocks = {}

  for i=0,22 do
    blocks[i] = Entity:create_entity()
    blocks[i]:add_component("sprite", {"tiles", "basic", {32,32}})
    blocks[i]:add_component("position", {32*i, 405-32, 1}) 
    blocks[i]:add_component("dimension", {32, 32})
    blocks[i]:add_component("body")
  end
end

local function load_player(x, y)
  local player = Entity:create_entity()
  player:add_component("player")
  player:add_component("sprite", {"guanaco", "basic", {80,60,0}})
  player:add_component("position", {x, y, 0}) 
  player:add_component("dimension", {80, 60})
  player:add_component("movement", {650, 400})
  player:add_component("body", {900, 900})

  animation_args = {}
  animation_args["frame_dimensions"] = {80, 60}
  animation_args["animations"] = {}
  animation_args["animations"][1] = {"run", {0,1,2,3,4}, 5}
  player:add_component("animation", animation_args) 
end

local function load_particles()
  local fire_emitter = Entity:create_entity()
  fire_emitter:add_component("particle_emitter", {"fire", 10})
  fire_emitter:add_component("position", {350, 290}) 
  fire_emitter:add_component("dimension", {16, 16})
end

local function load()
  load_assets()
  load_background()
  load_player(100, 405-64-61)
  --load_blocks()
  Map.load('scripts.level2.map')
  --load_particles()
end

local function update(dt)
end

local function render(dt)
end

M.load = load
M.update = update
M.render = render
return M

