local entity = create_entity()

local animations = Animation.new()
animations:add("walk-down", AnimationData.new({{ 1, 2, 3, 4, 5, 6, 7, 8 }}, 0.1, true))
animations.current = "walk-down"

add_component_position(entity, Position.new(600, 100))
add_component_sprite(entity, Sprite.new("spritesheet-nikte"))
add_component_animation(entity, animations)

--local pos = get_component_position(entity)
--local sprite = get_component_sprite(entity)
--sprite:set_frame(18)

--print (pos.x)
--print (pos.y)
--print (pos.z)

--remove_component_position(entity)
