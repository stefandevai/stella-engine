local M = {}

local Entity = require('scripts.Entity')

local SCREEN_WIDTH = 896
local SCREEN_HEIGHT = 504
local TILE_DIMENSION = 32
local SCREEN_WIDTH_TILES = SCREEN_WIDTH/TILE_DIMENSION
local SCREEN_HEIGHT_TILES = SCREEN_HEIGHT/TILE_DIMENSION 
local SCREEN_WIDTH_TILES_WITH_OFFSET = SCREEN_WIDTH_TILES + 1
local SCREEN_HEIGHT_TILES_WITH_OFFSET = SCREEN_HEIGHT_TILES + 1

M.offset = {}
M.offset.x = 0
M.offset.y = 0

M.chunk_counter = 0

local map
local tiles = {}
local current_tile_height = 1

local function add_tile(x, y)
  local tile = Entity:create_entity()
  tile.x = x*TILE_DIMENSION + M.offset.x
  tile.y = SCREEN_HEIGHT - (y+1)*TILE_DIMENSION + M.offset.y

  tile:add_component("tile")
  tile:add_component("sprite", {
    texture = "tiles",
    layer = "tiles",
    frame_dimensions = {TILE_DIMENSION,TILE_DIMENSION},
  })
  tile:add_component("position", {tile.x, tile.y, 1}) 
  tile:add_component("dimension", {TILE_DIMENSION, TILE_DIMENSION})
  tile:add_component("body")
end

local function plain(chunk_size, gap)
  local chunk_size = get_perlin_int(8, SCREEN_WIDTH_TILES/2, M.chunk_counter)
  local gap = get_perlin_int(1, 4, chunk_size*47)
  local height = get_random_int(1, current_tile_height+2)
  current_tile_height = height

  for x = gap, chunk_size+gap do
    add_tile(x, height)
  end

  return chunk_size + gap
end

local function elevate()
  local gap = 0
  local height = get_random_int(1, 5)
  local chunk_size = height*2

  local counter = 1
  for y = 1, height do
    local step = current_tile_height + y
    add_tile(counter, step)
    add_tile(counter+1, step)
    counter = counter + 2
  end

  current_tile_height = current_tile_height + height
  return chunk_size + gap
end

local function descend()
  local gap = 0
  local height = get_random_int(1, 5)
  local chunk_size = height*2

  local counter = 1
  for y = 1, height do
    local step = current_tile_height - y
    add_tile(counter, step)
    add_tile(counter+1, step)
    counter = counter + 2
  end

  current_tile_height = current_tile_height - height
  return chunk_size + gap
end

local function towers()
  local chunk_size = get_perlin_int(10, SCREEN_WIDTH_TILES, M.chunk_counter)
  local gap = get_perlin_int(0, 3, chunk_size*47)
  local i = gap

  while (i < chunk_size+gap) do
    local tower_length = get_random_int(2, 4)
    local tower_gap = get_random_int(2, 4)
    local tower_height = get_random_int(1, current_tile_height+4)
    current_tile_height = tower_height

    for x = 1, tower_length do
      for y = 0, tower_height do
        if x == 1 or x == tower_length or y == tower_height then
          add_tile(i+x, y)
        end
      end
    end

    i = i + tower_length + tower_gap
  end

  return chunk_size + gap
end

local chunk_generators = {
  plain,
  elevate,
  descend,
  towers
}

local function generate_chunk(xoffset, yoffset)
  local t = get_random_int(1, 4)
  local chunk_offset = chunk_generators[t]()
  M.offset.x = M.offset.x + chunk_offset*TILE_DIMENSION
  M.chunk_counter = M.chunk_counter + 1
end

function M.load()
  for i = 0, SCREEN_WIDTH_TILES-4 do
    local tile = Entity:create_entity()
    tile.x = i*TILE_DIMENSION
    tile.y = SCREEN_HEIGHT - 2*TILE_DIMENSION

    tile:add_component("tile")
    tile:add_component("sprite", {
      texture = "tiles",
      layer = "tiles",
      frame_dimensions = {TILE_DIMENSION,TILE_DIMENSION},
    })
    tile:add_component("position", {tile.x, tile.y, 1}) 
    tile:add_component("dimension", {TILE_DIMENSION, TILE_DIMENSION})
    tile:add_component("body")
  end

  M.offset.x = M.offset.x + (SCREEN_WIDTH_TILES-4)*32

  generate_chunk()
end

function M.update(camerax)
  if camerax + SCREEN_WIDTH*1.5 > M.offset.x then
    generate_chunk()
  end
end

return M

