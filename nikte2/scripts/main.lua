local entity = create_entity()
add_component_position(entity, Position.new(600, 100))
add_component_sprite(entity, Sprite.new("spritesheet-nikte"))

local pos = get_component_position(entity)
local sprite = get_component_sprite(entity)

sprite:set_frame(18)
print (pos.x)
print (pos.y)
print (pos.z)

--remove_component_position(entity)
