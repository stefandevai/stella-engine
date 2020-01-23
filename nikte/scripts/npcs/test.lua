--string = require("string")

local dialogs = {}
dialogs["hi"] = "Hi, how are you doing?"
dialogs["job"] = "I'm a peasant."
dialogs["age"] = "I'm 23 years old."

function talk (input)
    local lower_input = string.lower(input)
    print(lower_input)
    
    if dialogs[lower_input] ~= nil then
        return dialogs[lower_input]
    else
        return "Sorry, I don't understand what you say."
    end
end
