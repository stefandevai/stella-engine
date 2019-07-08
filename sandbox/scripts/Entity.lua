local Entity = {}
Entity.__index = Entity

function Entity:add_component(type, args)
  local obj = {}
  obj.id = self.id
  obj.type = type
  if args == nil then
    obj.args = {}
  else
    obj.args = args
  end
  e_add_component(obj)
end

function Entity:create_entity()
  local e = {}
  setmetatable(e, Entity)
  e.id = e_create_entity()
  return e
end

return Entity
