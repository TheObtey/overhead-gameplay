
local acmPlayer
local oCameraRoot
local oCamera
local oMovementComponent
local oLookComponent

local function InitializeRigidbody(iBodyType, iMass, bGravityEnabled, tAngularAxisLock, iLinearDamping, iAngularDamping)
    self:SetBodyType(iBodyType or 2)
    self:SetMass(iMass or 10)
    self:SetIsGravityEnabled(bGravityEnabled or true)

    local tAngularAxisLock = tAngularAxisLock or {}
    local x = type(tAngularAxisLock[1]) == "boolean" and tAngularAxisLock[1] or true
    local y = type(tAngularAxisLock[2]) == "boolean" and tAngularAxisLock[2] or false
    local z = type(tAngularAxisLock[3]) == "boolean" and tAngularAxisLock[3] or true
    self:LockAngularAxis(x, y, z)

    self:SetLinearDamping(iLinearDamping or 1)
    self:SetAngularDamping(iAngularDamping or 0.5)
end

local function InitializeActionMap()
    acmPlayer = actionmap:new("PLAYER")
    acmPlayer:CreateAction("MOVE_FORWARD", 1, 90)
    acmPlayer:CreateAction("MOVE_BACKWARD", 1, 83)
    acmPlayer:CreateAction("MOVE_LEFT", 1, 81)
    acmPlayer:CreateAction("MOVE_RIGHT", 1, 68)
    -- acmPlayer:CreateAction("LOOK", 3, 4080)
end

local function BindActions(oMovementComponent, oLookComponent)
    if oMovementComponent then
        acmPlayer:GetAction("MOVE_FORWARD").Event = oMovementComponent.MoveForward or function() print("PlayerBase: Move forward callback missing") end
        acmPlayer:GetAction("MOVE_BACKWARD").Event = oMovementComponent.MoveBackward or function() print("PlayerBase: Move backward callback missing") end
        acmPlayer:GetAction("MOVE_LEFT").Event = oMovementComponent.MoveLeft or function() print("PlayerBase: Move left callback missing") end
        acmPlayer:GetAction("MOVE_RIGHT").Event = oMovementComponent.MoveRight or function() print("PlayerBase: Move right callback missing") end
    else
        print("PlayerBase: MovementComponent missing")
    end

    if oLookComponent then
        acmPlayer:GetAction("LOOK").Event = oLookComponent.OnMouseMoveCallback or function() print("PlayerBase: Mouse move callback missing") end
    else
        print("PlayerBase: LookComponent missing")
    end
end

function OnInit()
    print("PlayerBase Initializing")

    InitializeRigidbody()

    oCameraRoot = self:GetNode("CameraRoot")
    oCamera = self:GetNode("CameraRoot/Camera")
    oMovementComponent = self:GetNode("components/MovementComponent")

    if oMovementComponent ~= nil then
        oMovementComponent:Setup(self)
    end

    oLookComponent = self:GetNode("components/LookComponent")

    if oLookComponent ~= nil then
        oLookComponent:Setup(self, oCameraRoot, oCamera)
    end

    InitializeActionMap()
    BindActions(oMovementComponent, _)
end

function OnUpdate(iDelta)
end