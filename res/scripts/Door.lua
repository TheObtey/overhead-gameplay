---@type NodeRigidBody

local self = self

local oReceiver
local oTarget

--Check if the door has to go to the origin or the target position
local bIsOpen = false

-- origin and target positions
local tOriginPos
local tTargetPos

--Callback to set the interaction to true when the tween has finish
local function ResetInteract()
    oReceiver:SetInteract(true)
end

local function UpdateTranslation(tValue)
    local vecNewPos = fmath.vec3:new(tValue[1], tValue[2], tValue[3])
    self:SetWorldPosition(vecNewPos)
end

--Interaction behaviour
function self:Interaction()
    oReceiver:SetInteract(false) --Stop the capacity to interact

    if bIsOpen == true then
        bIsOpen = false
        tween.Create("Tween:Door:translation", tTargetPos, tOriginPos, 2, ease.Out.Back, UpdateTranslation, ResetInteract)
    else
        bIsOpen = true
        tween.Create("Tween:Door:translation", tOriginPos, tTargetPos, 2, ease.Out.Back , UpdateTranslation, ResetInteract)
    end
end

function OnInit()
    oTarget = self:GetNode("/SceneRoot/targets/door"):As(NodeTypes.NODE3D)

    local compContainer = self:FindChild("components")
    oReceiver = compContainer:FindChild("InteractReceiverComponent")

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
    tween.Tick(iDelta)
end

function OnDestroy()
end