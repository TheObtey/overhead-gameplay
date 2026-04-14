---@class node
local self = self

local iMoveSpeed
local oRB

function self:MoveForward(icForward) print("move forward")
    if not oRB then return end

    oRB:ApplyLocalForceAtCenterOfMass(fmath.vec3:new(0, 0, -iMoveSpeed))

    print(oRB:GetPosition().x, oRB:GetPosition().y, oRB:GetPosition().z)
    print("\n\n")
end

function self:MoveBackward(icBackward) print("move backward")
    if not oRB then return end

    oRB:ApplyLocalForceAtCenterOfMass(fmath.vec3:new(0, 0, iMoveSpeed))

    print(oRB:GetPosition().x, oRB:GetPosition().y, oRB:GetPosition().z)
    print("\n\n")
end

function self:MoveLeft(icLeft) print("move left")
    if not oRB then return end

    oRB:ApplyLocalForceAtCenterOfMass(fmath.vec3:new(-iMoveSpeed, 0, 0))

    print(oRB:GetPosition().x, oRB:GetPosition().y, oRB:GetPosition().z)
    print("\n\n")
end

function self:MoveRight(icRight) print("move right")
    if not oRB then return end

    oRB:ApplyLocalForceAtCenterOfMass(fmath.vec3:new(iMoveSpeed, 0, 0))

    print(oRB:GetPosition().x, oRB:GetPosition().y, oRB:GetPosition().z)
    print("\n\n")
end

function self:SetMoveSpeed(iNewMoveSpeed)
    assert(iMoveSpeed ~= nil and type(iMoveSpeed) == "number", "MovementComponent: Valid number expected for move speed")
    iMoveSpeed = iNewMoveSpeed
end

function self:GetMoveSpeed()
    return iMoveSpeed
end

function self:Setup(oNewRigidBody, iNewMoveSpeed)
    assert(oNewRigidBody ~= nil, "MovementComponent: Valid rigidbody must be provided")

    oRB = oNewRigidBody
    iMoveSpeed = iNewMoveSpeed or 3000

    print("MovementComponent Initialized")
end

function OnInit()
end

function OnUpdate(iDelta)
end