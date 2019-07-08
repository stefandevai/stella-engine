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

function M.load(map_path, offset)
  map = require(map_path)
  if offset == nil then
    offset = {0.0, 0.0}
  end
  for i = 0, SCREEN_WIDTH_TILES_WITH_OFFSET * SCREEN_HEIGHT_TILES_WITH_OFFSET do
    local height = i // SCREEN_WIDTH_TILES_WITH_OFFSET
    local width = i % SCREEN_WIDTH_TILES_WITH_OFFSET

    -- If the index is within the map widths range
    if (width <= map.width and height < map.height) then
      local tile_type = map.data[width + map.width*height]

      if (tile_type == 1) then
        local tile = Entity:create_entity()
        tile.x = width*TILE_DIMENSION + M.offset.x
        tile.y = height*TILE_DIMENSION + M.offset.y

        tile:add_component("tile")
        tile:add_component("sprite", {
          texture = "tiles",
          layer = "scene",
          frame_dimensions = {TILE_DIMENSION,TILE_DIMENSION},
        })
        tile:add_component("position", {offset[1] + tile.x, offset[2] + tile.y, 1}) 
        tile:add_component("dimension", {TILE_DIMENSION, TILE_DIMENSION})
        tile:add_component("body")
        --tile:add_component("scroll", {-100,0})
        tiles[#tiles + 1] = tile
      end
    -- If the map height is less than screen height
    elseif (map.height < height) then
      break
    end
  end
end

function M.update()
  for i=1,#tiles do
    local tile = tiles[i]
    tile.x = tile.x - M.offset.x
    tile.y = tile.y - M.offset.y
    
    tiles[i]:update_component("position", {tile.x, tile.y, 1}) 
  end
end

return M
