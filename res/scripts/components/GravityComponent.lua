local oRB
local pRootNode

local GRAVITY_UP      = -450.0   -- montée légère (saut)
local GRAVITY_DOWN    = -650.0   -- descente rapide
local GRAVITY_FALLING = -950.0   -- chute libre

local iMass = 1

self.gravity = 1

local function ApplyCustomGravity(iDeltaTime)
    if not self.oRigidbody then return end
    if not self.oRigidbody:GetLinearVelocity() then return end

    local vel = self.oRigidbody:GetLinearVelocity()
    local gravity

    if vel.y > 0 then
        gravity = GRAVITY_UP
    elseif vel.y < -0.1 then
        gravity = GRAVITY_FALLING
    else
        gravity = GRAVITY_DOWN
    end

    local fGravityForce = gravity * iMass * iDeltaTime * self.gravity
    local vecForce = fmath.vec3:new(0, fGravityForce, 0)

    -- print(vecForce.x, vecForce.y, vecForce.z)

    self.oRigidbody:ApplyWorldForceAtCenterOfMass(vecForce)
end

function self:SetGravityDirection(iNewGravity)
    self.gravity = iNewGravity
end

function self:Setup(oNewParent, iNewGravity)
    self.oRigidbody = oNewParent:As(NodeTypes.NODE_RIGIDBODY)
    self.gravity = iNewGravity
end

function OnInit()
end

function OnUpdate(iDeltaTime)
    ApplyCustomGravity(iDeltaTime)
end