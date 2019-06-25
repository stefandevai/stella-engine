local Entity = {}
Entity.__index = Entity

local function add_sprite(self, args)
  local obj = {}
  obj["index"] = self.index
  obj["version"] = self.version
  obj["texture_name"] = args[1]
  if args[2] == nil then
    obj["type"] = 0
    e_add_sprite_component(obj)
  else
    obj["type"] = 1
    obj["frame_dimensions"] = args[2]
    e_add_sprite_component(obj)
  end
end

local function add_animation(self, args)
  local obj = {}
  obj["index"] = self.index
  obj["version"] = self.version
  obj["frame_dimensions"] = args["frame_dimensions"]
  obj["animations"] = args["animations"]
  e_add_animation_component(obj)
end

function Entity:add_component (type, args)
  if type == "sprite" then
    add_sprite(self, args)
  elseif type == "position" then
    e_add_position_component(self.index, self.version, args[1], args[2])
  elseif type == "dimension" then
    e_add_dimension_component(self.index, self.version, args[1], args[2])
  elseif type == "animation" then
    add_animation(self, args)
  else
    print("ERROR: There is no component called "..type)
  end
end

function Entity:create_entity()
  local e = {}
  setmetatable(e, Entity)
  e.index, e.version = e_create_entity()
  return e
end

return Entity
