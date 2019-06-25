local M = {}

local MOUNTAIN_LAYERS = 3

local Entity = require('scripts.entity')
local sky = Entity:create_entity()
local moon = Entity:create_entity()
local mountains = {}

local function load_assets()
  load_texture("sky", "assets/sprites/sky_background.png")
  load_texture("moon", "assets/sprites/moon_anim.png")
  load_texture("first-mountain", "assets/sprites/mountain3-bg.png")
  load_texture("middle-mountain", "assets/sprites/mountain2-bg.png")
  load_texture("last-mountain", "assets/sprites/mountain1-bg.png")

  load_texture("tiles", "assets/sprites/tiles.png")
  load_texture("over-block", "assets/sprites/over_block.png")
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

  for i=1,MOUNTAIN_LAYERS*2 do
    local tex_name, y, height, speed = nil, nil, nil, nil

    if i <= 2 then
      tex_name = "last-mountain"
      height = 170
      y = 230
    elseif i <= 4 then
      tex_name = "middle-mountain"
      height = 190
      y = 215
    else
      tex_name = "first-mountain"
      height = 230
      y = 175
    end
    mountains[i] = Entity:create_entity()
    mountains[i]:add_component("sprite", {tex_name})
    mountains[i]:add_component("position", {0 + ((i+1)%2)*720, y}) 
    mountains[i]:add_component("dimension", {720, height})
  end
end

local function load_blocks()
  local blocks = {}
  local over_blocks = {}

  for i=0,22 do
    blocks[i] = Entity:create_entity()
    blocks[i]:add_component("sprite", {"tiles", {32,32}})
    blocks[i]:add_component("position", {32*i, 405-64}) 
    blocks[i]:add_component("dimension", {32, 32})

    blocks[i*2] = Entity:create_entity()
    blocks[i*2]:add_component("sprite", {"tiles", {32,32}})
    blocks[i*2]:add_component("position", {32*i, 405-32}) 
    blocks[i*2]:add_component("dimension", {32, 32})
  end

  for i=1,2 do
    over_blocks[i] = Entity:create_entity()
    over_blocks[i]:add_component("sprite", {"over-block"})
    over_blocks[i]:add_component("position", {0 + ((i+1)%2)*720, 405-76}) 
    over_blocks[i]:add_component("dimension", {720, 12})
  end
end

local function load()
  load_assets()
  load_background()
  load_blocks()
end

local function update(dt)
end

local function render(dt)
end

M.load = load
M.update = update
M.render = render
return M

