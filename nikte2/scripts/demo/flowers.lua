local flowers = {}
local NUMBER_OF_FLOWERS = 10

local Entity = require('scripts.Entity')

local function load_flowers()
    local flower = Entity:create_entity()
    flower:add_component("position", {200, 200, 2})
    flower:add_component("dimension", {16, 16})
    flower:add_component("sprite", {
        texture = "flowers",
        layer = "tiles",
        frame_dimensions = {16, 16, 0},
        frame = 0,
    })

    local flower2 = Entity:create_entity()
    flower2:add_component("position", {216, 200, 2})
    flower2:add_component("dimension", {16, 16})
    flower2:add_component("sprite", {
        texture = "flowers",
        layer = "tiles",
        frame_dimensions = {16, 16, 0},
        frame = 5,
    })

    local flower3 = Entity:create_entity()
    flower3:add_component("position", {232, 200, 2})
    flower3:add_component("dimension", {16, 16})
    flower3:add_component("sprite", {
        texture = "flowers",
        layer = "tiles",
        frame_dimensions = {16, 16, 0},
        frame = 10,
    })

    local flower4 = Entity:create_entity()
    flower4:add_component("position", {248, 200, 2})
    flower4:add_component("dimension", {16, 16})
    flower4:add_component("sprite", {
        texture = "flowers",
        layer = "tiles",
        frame_dimensions = {16, 16, 0},
        frame = 15,
    })
end

function flowers:load()
    load_texture("flowers", "assets/sprites/flowers.png")
    load_flowers()
end

function flowers:update()
end

return flowers
