local M = {}

local function add_component(type, args)
  if type == "sprite" then
    e_add_sprite_component(0, 1, args[1])
  elseif type == "position" then
    e_add_position_component(0, 1, args[1], args[2])
  elseif type == "dimension" then
    e_add_dimension_component(0, 1, args[1], args[2])
  else
    print("ERROR: There is no component called " .. type)
  end
end

local entity_base = {}
entity_base.add_component = add_component

function M.create_entity()
  entity = entity_base
  entity.index, entity.version = e_create_entity()
  return entity
end

return M
