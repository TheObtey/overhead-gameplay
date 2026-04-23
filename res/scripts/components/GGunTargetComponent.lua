-- local self = self
local bCanInteract = true
local oCompContainer
local oBase
local sName

-- Set bCanInteract
function self:SetInteract(bNewCanInteract)
    bCanInteract = bNewCanInteract
end

-- Get bCanInteract
function self.CanInteract()
    return bCanInteract
end

--Function to implement in the parent script
-- function self.GravityGunInteraction()
--     if not oBase or not oBase:Is(NodeTypes.NODE_RIGIDBODY) then return end
--     print("Name oBase : "..oBase:GetName())
--     oBase:GravityGunUse()
-- end
function self.GravityGunGrabb()
    if not oBase or not oBase:Is(NodeTypes.NODE_RIGIDBODY) then return end
    print("GravityGunGrabb | oBase : "..oBase:GetName())
    oBase:GetGrabbed()
end
function self.GravityGunThrow()
    if not oBase or not oBase:Is(NodeTypes.NODE_RIGIDBODY) then return end
    print("GravityGunThrow |  oBase : "..oBase:GetName())
    oBase:GetThrown()
end

--Return the prompt to show when the raycast point to the node
function self:GetPrompt()
    return "Use Gravity Gun to interact with "..sName
end

function OnInit()
    oCompContainer = self:GetParent()
    oBase = oCompContainer:GetParent():As(NodeTypes.NODE_RIGIDBODY)
    sName = oBase:GetName()
end

function OnUpdate(dt) end

function OnDestroy() end