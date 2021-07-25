local entity = create_entity()

local animations = Animation.new()
animations:add("walk-down", AnimationData.new({{ 1, 2, 3, 4, 5, 6, 7, 8 }}, 0.1, true))
animations.current = "walk-down"

registry.assign["position"](entity, Position.new(600, 100))
registry.assign["sprite"](entity, Sprite.new("spritesheet-nikte"))
registry.assign["animation"](entity, animations)

-- local pos = component.get["position"](entity)
-- local sprite = component.get["sprite"](entity)
-- sprite:set_frame(18)

-- print (pos.x)
-- print (pos.y)
-- print (pos.z)

-- component.remove["position"](entity)

local entity2 = create_entity()
registry.assign["position"](entity2, Position.new(0, 0))
registry.assign["text"](entity2, Text.new("(Nikte$#!)", "font-lato", 48))
