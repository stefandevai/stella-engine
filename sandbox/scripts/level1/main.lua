local M = {}

local Entity = require('scripts.Entity')
local Map = require('scripts.ProceduralMap')
local Player = Entity:create_player()

local function load_assets()
  load_texture("sky", "assets/sprites/starry_sky.png")
  load_texture("moon", "assets/sprites/moon_anim.png")
  load_texture("tiles", "assets/sprites/tiles32x32.png")
  load_texture("guanaco", "assets/sprites/guanaco_frames2.png")
  load_texture("mountains1", "assets/sprites/mountains1.png")
  load_texture("mountains2", "assets/sprites/mountains2.png")
  load_texture("mountains3", "assets/sprites/mountains3.png")
  load_texture("fire-particle", "assets/sprites/fire-particle.png")
  load_texture("snowflake", "assets/snowflakes/flake2-small.png")
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

  animation_args = {}
  animation_args["frame_dimensions"] = {85, 85}
  animation_args["animations"] = {}
  animation_args["animations"][1] = {"glow", {3, 0, 4, 2, 1, 4, 3, 0, 2, 4, 3}, 20}
  moon:add_component("animation", animation_args) 

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
    mountains[i]:add_component("tileview")
    mountains[i]:add_component("scroll", scroll)
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
  Player:add_component("movement", { speed = {650, 400} })
  Player:add_component("body", {
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
  Player:add_component("animation", animation_args) 
end

local function load()
  create_layer({
    name = "background",
    priority = 0,
    shader = "basic",
    fixed = true,
  })

  create_layer({
    name = "tiles",
    priority = 1,
    shader = "basic",
    fixed = false,
  })

  create_layer({
    name = "particles",
    priority = 2,
    shader = "bloom",
    fixed = true,
  })

  local fire_emitter = Entity:create_entity()
  fire_emitter:add_component("particle_emitter", {type = "fire", quantity = 1})
  fire_emitter:add_component("position", {350, 290}) 
  fire_emitter:add_component("dimension", {16, 16})

  load_assets()
  load_background()
  load_player(300, 405-64-61)
  Map.load()
end

--local camerax = 0.0
--local speedx = 100.0
--local frame_counter = 1
local function update(dt)
  --local player_position = {get_position(Player.id)}
  --local camera_position = 0
  --if player_position[2] - 250 < 0 then
    --camera_position = player_position[2] - 250
  --end
  --camerax = camerax + speedx*dt
  --update_camera(camerax, camera_position, 0)
  --Map.update(camerax)

  --if frame_counter % 450 == 0 and speedx < 300.0 then
    --speedx = speedx + 7.0
    --print('Distance: ' .. camerax)
    --print('Speed: ' .. speedx)
  --end
  --frame_counter = frame_counter + 1
end

local function render(dt)
end

M.load = load
M.update = update
M.render = render
return M

