---@type NodeRigidBody

local self = self
local activatorComponent

function Interaction()
    if not activatorComponent then return end
    
end

function OnInit()
    local compContainer = self:GetChild("components")
    activatorComponent = compContainer:GetChild("ActivationComponent")
end

function OnUpdate(dt)
end

function OnDestroy()
end
