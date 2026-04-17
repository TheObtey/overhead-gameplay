local self = self
local bCanInteract = true
local oCompContainer
local oBase
local sName

-- Set bCanInteract
function self:SetInteract(bNewCanInteract)
    bCanInteract = bNewCanInteract
end

-- Get bCanInteract
function self:CanInteract()
    return bCanInteract
end

--Function to implement in the parent script
function self:Interact()
    if not oBase or not oBase:Is(NodeTypes.NODE_RIGIDBODY) then return end
    oBase:Interaction()
end

--Return the prompt to show when the raycast point to the node
function self:GetPrompt()
    return "Interact with "..sName.." [F]"
end

function OnInit()
    oCompContainer = self:GetParent()
    oBase = oCompContainer:GetParent():As(NodeTypes.NODE_RIGIDBODY)
    sName = oBase:GetName()
end

function OnUpdate(dt) end

function OnDestroy() end