
---@type Node3D
self = Node

local ActionMap = ActionMap:new()

function OnInit()
    local action = Action:new(ControlType.BUTTON, EventInput.KEY_SPACE)
    
    ActionMap:Emplace("JUMP", action)


end

function OnUpdate(dt)
end

function OnDestroy()

end
