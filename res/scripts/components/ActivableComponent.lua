---@type Node
local self = self
local oBase

function self:DoAction()
    if not oBase or not oBase:Is(NodeTypes.NODE_RIGIDBODY) then return end
    oBase:DoAction()
end

function OnInit()
    local oCompContainer = self:GetParent()
    oBase = oCompContainer:GetParent():As(NodeTypes.NODE_RIGIDBODY)
end

function OnUpdate(iDelta) end

function OnDestroy() end