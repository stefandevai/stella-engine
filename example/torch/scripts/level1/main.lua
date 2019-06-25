local M = {}

local stella = require('scripts.stella')

local sky = stella.create_entity()
local moon = stella.create_entity()

local function load_assets()
  load_texture("sky2", "assets/sprites/sky_background.png")
end

local function load_background()
  sky.add_component("sprite", {"sky2"})
  sky.add_component("dimension", {720, 405}) 
  sky.add_component("position", {0, 0}) 
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

