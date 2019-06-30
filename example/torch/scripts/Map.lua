local M = {}

local Entity = require('scripts.Entity')

local SCREEN_WIDTH = 896
local SCREEN_HEIGHT = 504
local TILE_DIMENSION = 32
local SCREEN_WIDTH_TILES = SCREEN_WIDTH/TILE_DIMENSION
local SCREEN_HEIGHT_TILES = SCREEN_HEIGHT/TILE_DIMENSION 
local SCREEN_WIDTH_TILES_WITH_OFFSET = SCREEN_WIDTH_TILES + 1
local SCREEN_HEIGHT_TILES_WITH_OFFSET = SCREEN_HEIGHT_TILES + 1

function M.load(map_path)
  map = require(map_path)
  for i = 0, SCREEN_WIDTH_TILES_WITH_OFFSET * SCREEN_HEIGHT_TILES_WITH_OFFSET do
    local height = i // SCREEN_WIDTH_TILES_WITH_OFFSET
    local width = i % SCREEN_WIDTH_TILES_WITH_OFFSET

    -- If the index is within the map widths range
    if (width <= map.width and height < map.height) then
      local tile_type = map.data[width + map.width*height]

      if (tile_type == 1) then
        local tile = Entity:create_entity()
        tile:add_component("sprite", {"tiles", "basic", {32,32}})
        tile:add_component("position", {width*TILE_DIMENSION, height*TILE_DIMENSION, 1}) 
        tile:add_component("dimension", {TILE_DIMENSION, TILE_DIMENSION})
        tile:add_component("body")
      end
    ---- If the map height is less than screen height
    elseif (map.height < height) then
      break
    end
  end
end

return M
