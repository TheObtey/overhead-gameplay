local bCanInteract = true
local parent
local name

function self:CanInteract()
    return bCanInteract
end

--Function to implement in the parent script
function self:Interact()
    parent:Interaction()
end

--Return the prompt to show when the raycast point to the node
function self:GetPrompt()
    return "Interact with "..name.." [F]"
end

function OnInit()
    parent = self:GetParent()
    name = parent:GetName()
end

function OnUpdate(dt)
end

function OnDestroy()
end