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

local function add_tile(x, y, z, body, frame)
  local tile = Entity:create_entity()
  tile.x = x*TILE_DIMENSION + M.offset.x
  tile.y = SCREEN_HEIGHT - (y+1)*TILE_DIMENSION + M.offset.y

  tile:add_component("tile")
  tile:add_component("sprite", {
    texture = "tiles",
    layer = "tiles",
    frame_dimensions = {TILE_DIMENSION,TILE_DIMENSION},
    frame = frame
  })
  tile:add_component("position", {tile.x, tile.y, z}) 
  tile:add_component("dimension", {TILE_DIMENSION, TILE_DIMENSION})

  if body == true then
    tile:add_component("body")
  end
end

local function get_available_matrix_index(matrix)
  local index = get_random_int(1, #matrix)
  while matrix[index] ~= -1 do
    index = get_random_int(1, #matrix, index)
  end
  return index
end

local function search_place(side, matrix, index, width)
  local available = true
  local index_height = index // width
  
  for y = 1, side do
    for x = 1, side do
      if matrix[index + x + (index_height+y)*width] ~= -1 then
        available = false
        break
      end
    end
  end

  -- tile is available for this index
  if available == true then
    local indexes = {}
    for y = 1, side do
      for x = 1, side do
        indexes[#indexes+1] = index + x + (index_height+y)*width
      end
    end
    return indexes
  end

  -- tile is not available for this index
  return {}
end

-- List indexes from top right to bottom left
local stone_tile_groups = {}
stone_tile_groups[1] = {1,10,16,24,27}
stone_tile_groups[2] = {{1,0,9,8}, {5,4,13,12}, {18,17,26,25}, {21,1,29,28}}
--stone_tile_groups[3] = {{11,3,2,19,18,17,27,26,25},  {3,2,11,5,4,19,26,25,1}, {11,3,2,19,23,22,27,31,30}, {11,3,2,19,7,6,27,15,14}}
stone_tile_groups[3] = {{11,3,2,19,18,17,27,26,25},  {3,2,11,5,4,19,26,25,1}, {11,3,2,19,23,22,27,31,30}}
stone_tile_groups["snow-top"] = {56,57,58,59,60,61,62}

local function build_nxn_tile(side, frames, width, nsearches)
  for i = 1, nsearches do
    local index = get_available_matrix_index(frames)
    local indexes = search_place(side, frames, index, width)
    if #indexes > 0 then
      --local group = stone_tile_groups[side][get_perlin_int(1,#stone_tile_groups[side],i)]
      --local group = stone_tile_groups[side][get_perlin_int(1,#stone_tile_groups[side])]
      local group = stone_tile_groups[side][get_random_int(1,#stone_tile_groups[side])]
      for n = 1, #indexes do
        frames[indexes[n]] = group[#indexes + 1 - n]
      end
    end
  end
  return frames
end

local function build_stone_tiles(bounding_matrix, width, height, gap)
  local y = 0
  local x = 1
  local frames = {}

  -- -1 means that the tile is available for adding other tiles
  for i = 1, #bounding_matrix do
    if bounding_matrix[i] ~= 0 then
      frames[i] = -1
    end
  end

  if height >= 3 and width >= 3 then
    frames = build_nxn_tile(3, frames, width, 10)
  end

  if height >= 2 and width >= 2 then
    frames = build_nxn_tile(2, frames, width, 10)
  end

  for i = 1, #bounding_matrix do
    if bounding_matrix[i] > 0 then
      x = (i-1) % width
      y = (i-1) // width

      local has_body = false
      if bounding_matrix[i] == 1 then
        has_body = true
      end

      if frames[i] == -1 then
        local frame = stone_tile_groups[1][get_perlin_int(1,#stone_tile_groups[1],x + y*width)]
        add_tile(gap+x, y, 1, has_body, frame)
      else
        add_tile(gap+x, y, 1, has_body, frames[i])
      end

      if y == height then
        -- Add snow on top of tile
        add_tile(gap+x, y, 2, false, 48)

        local snow_top = stone_tile_groups["snow-top"][get_random_int(1,#stone_tile_groups["snow-top"])]
        add_tile(gap+x, y+1, 2, false, snow_top)
      end

    end
  end
end

local function build_stair_tiles(bounding_matrix, width, height, gap)
  local y = 0
  local x = 1

  for i = 1, #bounding_matrix do
    if bounding_matrix[i] > 0 then
      x = (i-1) % width
      y = (i-1) // width

      local has_body = false
      if bounding_matrix[i] == 1 then
        has_body = true
      end

      if y == height then
        add_tile(gap+x, y, 1, has_body, 20)

        -- Add snow on top of tile
        add_tile(gap+x, y, 2, true, 48)
        local snow_top = stone_tile_groups["snow-top"][get_random_int(1,#stone_tile_groups["snow-top"])]
        add_tile(gap+x, y+1, 2, false, snow_top)
      else
        local frame = stone_tile_groups[1][get_perlin_int(1,#stone_tile_groups[1],1+x + y*width)]
        add_tile(gap+x, y, 1, has_body, frame)
      end
    end
  end
end

local function plain()
  local height = get_random_int(1, current_tile_height+2)
  local chunk_size = get_random_int(15, SCREEN_WIDTH_TILES)
  local gap = get_perlin_int(1, 4, chunk_size*47)
  current_tile_height = height

  local bounding_matrix = {}
  

  for y = 0, height do
    for x = 1, chunk_size do
      if x == 1 or x == chunk_size or y == height then
        bounding_matrix[x + y*chunk_size] = 1
      else
        bounding_matrix[x + y*chunk_size] = 2
      end
    end
  end

  build_stone_tiles(bounding_matrix, chunk_size, height, gap)

  return chunk_size + gap
end

local function elevate()
  local height = get_random_int(1, 5)
  local chunk_size = height*2

  local counter = 0
  for y = 1, height do

    local bounding_matrix = {}

    for y2 = 0, current_tile_height + y do
      if y == height then
        bounding_matrix[0 + y2*2] = 2
        bounding_matrix[1 + y2*2] = 1
      else
        bounding_matrix[0 + y2*2] = 2
        bounding_matrix[1 + y2*2] = 2
      end
    end

    local step = current_tile_height + y

    bounding_matrix[1 + step*2] = 1
    bounding_matrix[2 + step*2] = 1

    build_stair_tiles(bounding_matrix, 2, current_tile_height+y, counter)

    counter = counter + 2
  end

  current_tile_height = current_tile_height + height
  return chunk_size
end

local function descend()
  local height = get_random_int(1, 5)
  local chunk_size = height*2

  local counter = 0
  for y = 1, height do

    local bounding_matrix = {}

    for y2 = 0, current_tile_height - y do
      if y == height then
        bounding_matrix[0 + y2*2] = 2
        bounding_matrix[1 + y2*2] = 1
      else
        bounding_matrix[0 + y2*2] = 2
        bounding_matrix[1 + y2*2] = 2
      end
    end

    local step = current_tile_height - y

    bounding_matrix[1 + step*2] = 1
    bounding_matrix[2 + step*2] = 1

    build_stair_tiles(bounding_matrix, 2, current_tile_height-y, counter)

    counter = counter + 2
  end

  current_tile_height = current_tile_height - height
  return chunk_size
end

local function towers()
  local chunk_size = get_perlin_int(10, SCREEN_WIDTH_TILES, M.chunk_counter)
  local gap = get_perlin_int(0, 2, chunk_size*47)
  local i = gap

  local gap_accumulation = gap
  while (i < chunk_size+gap) do
    local bounding_matrix = {}
    local tower_length = get_random_int(2, 4)
    local tower_gap = get_random_int(1, 5)
    local tower_height = get_random_int(1, current_tile_height+2)
    current_tile_height = tower_height

    for x = 1, tower_length do
      for y = 0, tower_height do
        if x == 1 or x == tower_length or y == tower_height then
          bounding_matrix[x + y*tower_length] = 1
        else
          bounding_matrix[x + y*tower_length] = 2
        end
      end
    end

    build_stone_tiles(bounding_matrix, tower_length, tower_height, gap_accumulation)

    i = i + tower_length + tower_gap
    gap_accumulation = gap_accumulation + tower_length + tower_gap
  end

  return gap_accumulation
end

local chunk_generators = {
  plain,
  elevate,
  descend,
  towers
}

local function generate_chunk(chunk_type)
  local t = chunk_type
  if t == nil then
    t = get_random_int(1, 4)
    if t == 3 and current_tile_height < 6 then
      t = 2
    end
  end
  local chunk_offset = chunk_generators[t]()
  M.offset.x = M.offset.x + chunk_offset*TILE_DIMENSION
  M.chunk_counter = M.chunk_counter + 1
end

function M.load()
  generate_chunk(1)
end

function M.update(camerax)
  if camerax + SCREEN_WIDTH*1.5 > M.offset.x then
    generate_chunk()
  end
end

return M

