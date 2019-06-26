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

  load_texture("guanaco", "assets/sprites/guanaco-anim.png")

  load_texture("fire-particle", "assets/sprites/fire-particle.png")
  load_texture("snowflake", "assets/snowflakes/flake2-small.png")

  load_font("font-cursive", "assets/sprites/cursive.png");
end

local function load_background()
  sky:add_component("sprite", {"sky"})
  sky:add_component("position", {0, 0, -10})
  sky:add_component("dimension", {720, 405})

  moon:add_component("sprite", {"moon", {85, 85, -9}})
  moon:add_component("position", {478, 78}) 
  moon:add_component("dimension", {85, 85}) 

  animation_args = {}
  animation_args["frame_dimensions"] = {85, 85}
  animation_args["animations"] = {}
  animation_args["animations"][1] = {"glow", {3, 0, 4, 2, 1, 4, 3, 0, 2, 4, 3}, 20}
  moon:add_component("animation", animation_args) 

  for i=1,MOUNTAIN_LAYERS*2 do
    local y, z = nil, nil
    local tex_name = nil
    local height, speed = nil, nil, nil, nil

    if i <= 2 then
      tex_name = "last-mountain"
      height = 170
      y = 230
      z = -8
      speed = {-3,0}
    elseif i <= 4 then
      tex_name = "middle-mountain"
      height = 190
      y = 215
      z = -7
      speed = {-9,0}
    else
      tex_name = "first-mountain"
      height = 230
      y = 175
      z = -6
      speed = {-15,0}
    end
    mountains[i] = Entity:create_entity()
    mountains[i]:add_component("sprite", {tex_name})
    mountains[i]:add_component("position", {0 + ((i+1)%2)*720, y, z}) 
    mountains[i]:add_component("dimension", {720, height})
    mountains[i]:add_component("tileview")
    mountains[i]:add_component("movement", speed)
  end
end

local function load_blocks()
  local blocks = {}
  local over_blocks = {}

  for i=0,22 do
    blocks[i] = Entity:create_entity()
    blocks[i]:add_component("sprite", {"tiles", {32,32}})
    blocks[i]:add_component("position", {32*i, 405-32, 1}) 
    blocks[i]:add_component("dimension", {32, 32})
    blocks[i]:add_component("body")
  end

  for i=1,2 do
    over_blocks[i] = Entity:create_entity()
    over_blocks[i]:add_component("sprite", {"over-block"})
    over_blocks[i]:add_component("position", {0 + ((i+1)%2)*720, 405-44, 1}) 
    over_blocks[i]:add_component("dimension", {720, 12})
    over_blocks[i]:add_component("tileview")
    over_blocks[i]:add_component("movement", {-180})
  end
end

local function load_player(x, y)
  local player = Entity:create_entity()
  player:add_component("player")
  player:add_component("sprite", {"guanaco", {80,60,0}})
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
  local snow_emitter = Entity:create_entity()
  snow_emitter:add_component("particle_emitter", {"snow", 10})
  snow_emitter:add_component("position", {0, -64}) 
  snow_emitter:add_component("dimension", {32, 32})

  --local fire_emitter = Entity:create_entity()
  --fire_emitter:add_component("particle_emitter", {"fire", 10})
  --fire_emitter:add_component("position", {350, 290}) 
  --fire_emitter:add_component("dimension", {16, 16})
end

--local function load_text()
  --local title = Entity:create_entity()
  --title:add_component("position", {30, 30}) 
  --title:add_component("dimension", {9, 9})
  --title:add_component("text", {"- TORCH -", "font-cursive", true})
--end

local function load()
  load_assets()
  load_background()
  load_player(100, 405-64-61)
  load_blocks()
  load_particles()
  --load_text()
end

local function update(dt)
end

local function render(dt)
end

M.load = load
M.update = update
M.render = render
return M

