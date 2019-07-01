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

local map
local tiles = {}

local function generate_chunk(xoffset, yoffset)
  local chunk_size = 10

  for i = 2, chunk_size do
    local tile = Entity:create_entity()
    tile.x = i*TILE_DIMENSION + M.offset.x
    tile.y = SCREEN_HEIGHT - 2*TILE_DIMENSION + M.offset.y

    tile:add_component("tile")
    tile:add_component("sprite", {
      texture = "tiles",
      layer = "scene",
      frame_dimensions = {TILE_DIMENSION,TILE_DIMENSION},
    })
    tile:add_component("position", {tile.x, tile.y, 1}) 
    tile:add_component("dimension", {TILE_DIMENSION, TILE_DIMENSION})
    tile:add_component("body")
  end
  M.offset.x = M.offset.x + 10*TILE_DIMENSION
end

function M.load()
  for i = 0, SCREEN_WIDTH_TILES-4 do
    local tile = Entity:create_entity()
    tile.x = i*TILE_DIMENSION
    tile.y = SCREEN_HEIGHT - 2*TILE_DIMENSION

    tile:add_component("tile")
    tile:add_component("sprite", {
      texture = "tiles",
      layer = "scene",
      frame_dimensions = {TILE_DIMENSION,TILE_DIMENSION},
    })
    tile:add_component("position", {tile.x, tile.y, 1}) 
    tile:add_component("dimension", {TILE_DIMENSION, TILE_DIMENSION})
    tile:add_component("body")
  end

  M.offset.x = M.offset.x + (SCREEN_WIDTH_TILES-4)*32

  generate_chunk()
  generate_chunk()
end

function M.update(camerax)
  if camerax + SCREEN_WIDTH*1.5 > M.offset.x then
    generate_chunk()
    print('generating chunk')
  end
end

return M

