---@type NodeRigidBody

local self = self
local oReceiver
local oTarget

local bIsOpen = false

-- origin and target positions
local tOriginPos
local tTargetPos

local function void() end

local function UpdateTranslation(tValue)
    local vecNewPos = fmath.vec3:new(tValue[1], tValue[2], tValue[3])
    self:SetWorldPosition(vecNewPos)
end

--Action behaviour
function self.DoAction()
    if bIsOpen == true then
        bIsOpen = false
        tween.Create("Tween:Door:translation", tTargetPos, tOriginPos, 2, ease.Out.Back, UpdateTranslation, void)
    else
        bIsOpen = true
        tween.Create("Tween:Door:translation", tOriginPos, tTargetPos, 2, ease.Out.Back , UpdateTranslation, void)
    end
end

function self:Interaction() self.DoAction() end

function OnInit()
    oTarget = self:GetNode("/SceneRoot/targets/door"):As(NodeTypes.NODE3D)

    local oCompContainer = self:FindChild("components")
    oReceiver = oCompContainer:FindChild("InteractReceiverComponent")

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
