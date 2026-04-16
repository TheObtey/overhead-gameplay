---@class node
self = self

local iMoveSpeed
local oRB

self.MoveForward = function(icForward)
    if not oRB then return end
    oRB:ApplyLocalForceAtCenterOfMass(fmath.vec3:new(0, 0, -iMoveSpeed))
end

self.MoveBackward = function(icBackward)
    if not oRB then return end
    oRB:ApplyLocalForceAtCenterOfMass(fmath.vec3:new(0, 0, iMoveSpeed))
end

self.MoveLeft = function(icLeft)
    if not oRB then return end
    oRB:ApplyLocalForceAtCenterOfMass(fmath.vec3:new(-iMoveSpeed, 0, 0))
end

self.MoveRight = function(icRight)
    if not oRB then return end
    oRB:ApplyLocalForceAtCenterOfMass(fmath.vec3:new(iMoveSpeed, 0, 0))
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
    iMoveSpeed = iNewMoveSpeed or 100

    print("MovementComponent Initialized")
end