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

local function add_position(self, args)
  local z = 0
  if args[3] ~= nil then
    z = args[3]
  end
  e_add_position_component(self.index, self.version, args[1], args[2], z)
end

local function add_animation(self, args)
  local obj = {}
  obj["index"] = self.index
  obj["version"] = self.version
  obj["frame_dimensions"] = args["frame_dimensions"]
  obj["animations"] = args["animations"]
  e_add_animation_component(obj)
end

local function add_tileview(self)
  local obj = {}
  obj["index"] = self.index
  obj["version"] = self.version
  e_add_tileview_component(obj)
end

local function add_movement(self, speed)
  local obj = {}
  obj["index"] = self.index
  obj["version"] = self.version
  obj["speedx"] = speed[1]
  obj["speedy"] = speed[2]
  e_add_movement_component(obj)
end

local function add_playerc(self)
  local obj = {}
  obj["index"] = self.index
  obj["version"] = self.version
  e_add_player_component(obj)
end

local function add_body(self, drag)
  local obj = {}
  obj["index"] = self.index
  obj["version"] = self.version
  if drag == nil then
    obj["dragx"] = 0
    obj["dragy"] = 0
  else
    obj["dragx"] = drag[1]
    obj["dragy"] = drag[2]
  end
  e_add_body_component(obj)
end

local function add_text(self, args)
  local obj = {}
  obj["index"] = self.index
  obj["version"] = self.version
  obj["text"] = args[1]
  obj["font_name"] = args[2]
  obj["is_static"] = args[3]
  e_add_text_component(obj)
end

local function add_particle_emitter(self, args)
  local obj = {}
  obj["index"] = self.index
  obj["version"] = self.version
  obj["type"] = args[1]
  obj["quantity"] = args[2]
  e_add_particle_emitter_component(obj)
end

function Entity:add_component (type, args)
  if type == "sprite" then
    add_sprite(self, args)
  elseif type == "position" then
    add_position(self, args)
  elseif type == "dimension" then
    e_add_dimension_component(self.index, self.version, args[1], args[2])
  elseif type == "animation" then
    add_animation(self, args)
  elseif type == "tileview" then
    add_tileview(self)
  elseif type == "movement" then
    add_movement(self, args)
  elseif type == "player" then
    add_playerc(self)
  elseif type == "body" then
    add_body(self, args)
  elseif type == "text" then
    add_text(self, args)
  elseif type == "particle_emitter" then
    add_particle_emitter(self, args)
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
