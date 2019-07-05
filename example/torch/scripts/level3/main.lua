local M = {}

local Entity = require('scripts.Entity')
local Map = require('scripts.ProceduralMap')
local Player = Entity:create_entity()

local function load_assets()
  load_texture("sky", "assets/sprites/starry_sky.png")
  load_texture("moon", "assets/sprites/moon_anim.png")
  load_texture("tiles", "assets/sprites/tiles32x32.png")
  load_texture("guanaco", "assets/sprites/guanaco_frames2.png")
  load_texture("mountains1", "assets/sprites/mountains1.png")
  load_texture("mountains2", "assets/sprites/mountains2.png")
  load_texture("mountains3", "assets/sprites/mountains3.png")
end

local function load_background()
  local moon = Entity:create_entity()
  moon:add_component("sprite", {
    texture = "moon",
    layer = "background",
    frame_dimensions = {85, 85, -9},
  })
  moon:add_component("position", {678, 78, -9}) 
  moon:add_component("dimension", {85, 85}) 

  local sky = Entity:create_entity()
  sky:add_component("sprite", {
    texture = "sky",
    layer = "background"
  })
  sky:add_component("position", {0, 0, -10}) 
  sky:add_component("dimension", {896, 504}) 

  local mountains = {}
  for i=1,3*2 do
    local y, z = nil, nil
    local tex_name = nil
    local height, scroll = nil, nil, nil, nil

    if i <= 2 then
      tex_name = "mountains3"
      z = -8
      scroll = {-2.0,0}
    elseif i <= 4 then
      tex_name = "mountains2"
      z = -7
      scroll = {-5.0,0}
    else
      tex_name = "mountains1"
      z = -6
      scroll = {-12.0,0}
    end
    mountains[i] = Entity:create_entity()
    mountains[i]:add_component("sprite", {
      texture = tex_name,
      layer = "background",
    })
    mountains[i]:add_component("position", {0 + ((i+1)%2)*896, 0, z}) 
    mountains[i]:add_component("dimension", {896, 504})
  end
end

local function load_player(x, y)
  Player:add_component("player")
  Player:add_component("sprite", {
    texture = "guanaco",
    layer = "tiles",
    frame_dimensions = {80,60,0},
  })
  Player:add_component("position", {x, y, 3}) 
  Player:add_component("dimension", {80, 60})
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
  load_player(100, 100)
end

local function update(dt)
end

local function render(dt)
end

M.load = load
M.update = update
M.render = render
return M

