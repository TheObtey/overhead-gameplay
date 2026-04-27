---@type NodeRigidBody

local self = self

-- origin and target positions
local tOriginPos
local tTargetPos

local function void() end

--Action behaviour
function self.DoAction()

end

function self:Interaction() self.DoAction() end

function OnInit()

    local vecOriginPos = self:GetWorldPosition()
    tOriginPos =
    {
        vecOriginPos.x,
        vecOriginPos.y,
        vecOriginPos.z
    }

    local vecTargetPos = oTarget:GetWorldPosition()

    tTargetPos = 
    {
        vecTargetPos.x,
        vecTargetPos.y,
        vecTargetPos.z
    }
end

function OnUpdate(iDelta)
end

function OnDestroy() end
