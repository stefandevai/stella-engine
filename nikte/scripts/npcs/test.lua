local dialogs = {}
dialogs["hi"] = "Hi, how are you doing?"

function talk (input)
    return dialogs[input]
end
