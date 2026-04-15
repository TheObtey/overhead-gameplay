---@type NodeRigidBody

local self = self
local oTarget

local function doShit()
    print(self:GetPosition().x, self:GetPosition().y, self:GetPosition().z)
end

function self:Interaction()
    print("Start Interaction")

    local vecFromPos = self:GetPosition()
    local tFrom = 
    {
        x = vecFromPos.x,
        y = vecFromPos.y,
        z = vecFromPos.z
    }

    local vecTargetPos = oTarget:GetWorldPosition()
    local tTo = 
    {
        x = vecTargetPos.x,
        y = vecTargetPos.y,
        z = vecTargetPos.z
    }
    print(tTo.x, tTo.y, tTo.z)
    tween.Create("Tween:Door:translation", tFrom, tTo, 3, ease.In.Back, doShit)
end

function OnInit()
    print(self:GetName())
    oTarget = self:FindChild("N3D_Target"):As(NodeTypes.NODE3D)
    print(oTarget:GetName())
end

function OnUpdate(dt)
end

function OnDestroy()
end
