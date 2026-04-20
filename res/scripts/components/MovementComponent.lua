
---@class noderigidbody
self = self

local iMoveSpeed
local iJumpForce
local oRB

-- ─── Gravité custom ───────────────────────────────────────────────────────────
local GRAVITY_UP      = -450.0   -- montée légère
local GRAVITY_DOWN    = -650.0   -- descente rapide
local GRAVITY_FALLING = -950.0   -- chute libre sans avoir sauté

-- ─── État interne ─────────────────────────────────────────────────────────────
local bIsGrounded     = true
local bIsMoving       = false
local bJumpedThisFrame = false

local GROUNDED_GRACE   = 0.2 -- secondes avant de considérer qu'on a quitté le sol
local fGroundedTimer   = GROUNDED_GRACE

-- Coyote time
local COYOTE_TIME     = 0.12
local fCoyoteTimer    = 0.0

-- Jump buffer
local JUMP_BUFFER     = 0.12
local fJumpBufferTimer = 0.0

-- Référence à la State Machine
local oSM = nil



-- ─── Mouvement ────────────────────────────────────────────────────────────────

self.MoveForward = function(icForward)
    -- print("FORWARD")
    if not oRB then return end
    bIsMoving = true
    oRB:ApplyLocalForceAtCenterOfMass(fmath.vec3:new(0, 0, -iMoveSpeed))
end

self.MoveBackward = function(icBackward)
    -- print("BACKARD")
    if not oRB then return end
    bIsMoving = true
    -- oRB:ApplyLocalForceAtCenterOfMass(fmath.vec3:new(0, 0, iMoveSpeed))
    oRB:SetLocalPosition(fmath.vec3:new(-5,2,5))
end

self.MoveLeft = function(icLeft)
    -- print("LEFT")
    if not oRB then return end
    bIsMoving = true
    oRB:ApplyLocalForceAtCenterOfMass(fmath.vec3:new(-iMoveSpeed, 0, 0))
end

self.MoveRight = function(icRight)
    -- print("RIGHT")
    -- print("Forces : {".. oRB:GetTotalForce().x .. ", ".. oRB:GetTotalForce().y .. ", ".. oRB:GetTotalForce().z .. "}")
    if not oRB then return end
    bIsMoving = true
    oRB:ApplyLocalForceAtCenterOfMass(fmath.vec3:new(iMoveSpeed, 0, 0))
end

-- ─── Saut ─────────────────────────────────────────────────────────────────────

self.Jump = function(icJump)
    if not oRB then return end
    if icJump:IsPressed() then
    fJumpBufferTimer = JUMP_BUFFER end
end

local function _TryJump()
    -- if fCoyoteTimer > 0 and fJumpBufferTimer > 0 then
    if fJumpBufferTimer > 0 then
        local vel = oRB:GetLinearVelocity()
        oRB:SetLinearVelocity(fmath.vec3:new(vel.x, 0, vel.z))
        oRB:ApplyLocalForceAtCenterOfMass(fmath.vec3:new(0, iJumpForce * oRB.gravity, 0))

        fJumpBufferTimer = 0
        fCoyoteTimer     = 0
        bIsGrounded      = false
        bJumpedThisFrame = true
        print("JUMPING : " .. iJumpForce)
        if oSM then oSM:TransitionTo("JUMP") end
    end
end

-- ─── Gravité custom ───────────────────────────────────────────────────────────


local function _ApplyCustomGravity(oMass, dt)
    local vel = oRB:GetLinearVelocity()
    local gravity

    if bIsGrounded then
        return
    elseif vel.y > 0 then
        gravity = GRAVITY_UP
    elseif vel.y < -0.1 and not bJumpedThisFrame then
        gravity = GRAVITY_FALLING
    else
        gravity = GRAVITY_DOWN
    end
    -- print("____________")
    -- print("Vel Y = ".. vel.y)
    -- print("Gravity = ".. gravity)
    local fGravityForce = gravity * oMass * 0.0005 * oRB.gravity
    -- print("Gravity = "..fGravityForce)
    oRB:ApplyWorldForceAtCenterOfMass(fmath.vec3:new(0, fGravityForce, 0))
end


-- ─── Update physique 


local bWasMovingLastFrame = false

function self:PhysicsUpdate(dt)
    if not oRB then return end
    local oMass = oRB:GetMass()

    -- Gestion sol avec grace period
    if fGroundedTimer > 0 then
        fGroundedTimer = fGroundedTimer - dt
        if fGroundedTimer <= 0 then
            bIsGrounded = false
        end
    end

    -- Coyote time
    if bIsGrounded then
        fCoyoteTimer = COYOTE_TIME
    else
        fCoyoteTimer = fmath.Max(0, fCoyoteTimer - dt)
    end
    fJumpBufferTimer = fmath.Max(0, fJumpBufferTimer - dt)

    _TryJump()
    _ApplyCustomGravity(oMass, dt)

    bWasMovingLastFrame = bIsMoving
    bIsMoving           = false
    bJumpedThisFrame    = false
end
-- ─── Getters pour la State Machine 

function self:IsGrounded()
    return bIsGrounded
end

function self:SetGrounded(bValue)
    if bValue then
        if not bIsGrounded then
            if oSM then oSM:TransitionTo("LAND") end
        end
        bIsGrounded    = true
        fGroundedTimer = GROUNDED_GRACE
    else
        -- Ne pas désactiver immédiatement, laisser le timer expirer ?
    end
end

-- function self:IsMoving()
--     return bIsMoving
-- end
function self:IsMoving()
    return bWasMovingLastFrame  
end

function self:GetVerticalVelocity()
    if not oRB then return 0 end
    return oRB:GetLinearVelocity().y
end

-- ─── Injection de la State Machine 

function self:SetStateMachine(oNewSM)
    oSM = oNewSM
    print("StateMachine set in MovementComponent")
end

-- ─── Setup 

function self:SetMoveSpeed(iNewMoveSpeed)
    assert(type(iNewMoveSpeed) == "number", "MovementComponent: Valid number expected for move speed")
    iMoveSpeed = iNewMoveSpeed
end

function self:GetMoveSpeed() return iMoveSpeed end

function self:SetJumpForce(iNewJumpForce)
    assert(type(iNewJumpForce) == "number", "MovementComponent: Valid number expected for jump force")
    iJumpForce = iNewJumpForce
end

function self:GetJumpForce() return iJumpForce end

function self:Setup(oNewRigidBody, iNewMoveSpeed, iNewJumpForce)
    assert(oNewRigidBody ~= nil, "MovementComponent: Valid rigidbody must be provided")

    oRB        = oNewRigidBody
    iMoveSpeed = iNewMoveSpeed or 15
    iJumpForce = iNewJumpForce or 6500  -- force * masse, pas impulsion directe

    print("MovementComponent Initialized")
end

