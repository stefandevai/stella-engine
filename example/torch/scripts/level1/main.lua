local M = {}

local stella = require('scripts.stella')

local sky = stella:create_entity()
local moon = stella:create_entity()

local function load_assets()
  load_texture("sky", "assets/sprites/sky_background.png")
  load_texture("moon", "assets/sprites/moon_anim.png")
end

local function load_background()
  sky:add_component("sprite", {"sky"})
  sky:add_component("position", {0, 0})
  sky:add_component("dimension", {720, 405})

  moon:add_component("sprite", {"moon", {85, 85}})
  moon:add_component("position", {478, 78}) 
  moon:add_component("dimension", {85, 85}) 

  animation_args = {}
  animation_args["frame_dimensions"] = {85, 85}
  animation_args["animations"] = {}
  animation_args["animations"][1] = {"glow", {3, 0, 4, 2, 1, 4, 3, 0, 2, 4, 3}, 20}
  moon:add_component("animation", animation_args) 
end

local function load()
  load_assets()
  load_background()
end

local function update(dt)
end

local function render(dt)
end

M.load = load
M.update = update
M.render = render
return M

