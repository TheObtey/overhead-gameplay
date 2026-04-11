local bCanInteract = true

function self:CanInteract()
    return bCanInteract
end

--Function to implement in the parent script
function self:Interact()
    local parent = self:GetParent()
    parent:Interaction()
end

--Return the prompt to show when the raycast point to the node
function self:GetPrompt()
    local parent = self:GetParent()
    local name = parent:GetName()
    
    return "Interact with "..name.." [F]"
end

function OnInit()
end

function OnUpdate(dt)
end

function OnDestroy()
end