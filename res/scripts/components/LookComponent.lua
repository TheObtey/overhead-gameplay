---@class node
local self = self

local iMouseSensitivity = 0.002
local iMinPitch = -89.0
local iMaxPitch = 89.0
local iPitch = 0.0

local oRB
local oCameraRoot
local oCamera

function self:OnMouseMoveCallback(icMouse) -- TODO: Waiting for Antoine's fix to continue this script
    -- local vecMouse = icontrol.ReadAsVec2(icMouse)
    -- print(vecMouse.x, vecMouse.y)

    -- if not oRB or not oCameraRoot then return end

    -- local vecPlayerUp = oRB:GetLocalUp()

    -- oRB:AddLocalPitch(-vecMouse.x * iMouseSensitivity)

    -- iPitch = iPitch - vecMouse.y * iMouseSensitivity
    -- iPitch = fmath.Clamp(iPitch, fmath.Rad(fmath.Deg(iMinPitch)), fmath.Rad(fmath.Deg(iMaxPitch)))

    -- oCameraRoot:AddLocalPitch(iPitch)
end

function self:Setup(oNewRigidBody, oNewCameraRoot, oNewCamera)
    oRB = oNewRigidBody
    oCameraRoot = oNewCameraRoot
    oCamera = oNewCamera

    print("LookComponent Initialized")
end

function OnInit()
end

function OnUpdate(iDelta)
end