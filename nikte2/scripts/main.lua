print("Hello, world!")

entity = create_entity()
sprite = sprite_component.new("nikte")
pos = position_component.new(100, 100)

add_component(entity, sprite)
add_component(entity, pos)
