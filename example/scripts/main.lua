local player = create_entity()

local player_animations = Animation.new()
player_animations:add("walk-down", AnimationData.new({{ 1, 2, 3, 4, 5, 6, 7, 8 }}, 0.1, true))
player_animations.current = "walk-down"

registry.assign["position"](player, Position.new(600, 100))
registry.assign["sprite"](player, Sprite.new("spritesheet-nikte"))
registry.assign["animation"](player, player_animations)

-- local mesh = create_entity()
-- registry.assign["mesh"](mesh, Mesh.new("skull"))
-- registry.assign["position"](mesh, Position.new(0, 0))

-- local pos = registry.get["position"](entity)
-- local sprite = registry.get["sprite"](entity)
-- sprite:set_frame(18)

-- print (pos.x)
-- print (pos.y)
-- print (pos.z)

-- registry.remove["position"](entity)

local test_text = create_entity()

registry.assign["position"](test_text, Position.new(20, 10))
registry.assign["text"](test_text, Text.new("Stella Engine", "font-lato", 32))

local text_component = registry.get["text"](test_text)
text_component:set_is_static(false)

