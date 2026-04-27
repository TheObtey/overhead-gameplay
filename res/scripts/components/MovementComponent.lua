
-- ---@class noderigidbody
-- self = self

local fMoveSpeed
local MOVE_SPEED
local fJumpForce
local JUMP_FORCE
local oRB

-- ─── Gravité custom ───────────────────────────────────────────────────────────
local GRAVITY_UP      = -450.0   -- montée légère (saut)
local GRAVITY_DOWN    = -650.0   -- descente rapide
local GRAVITY_FALLING = -950.0   -- chute libre

-- ─── État interne ─────────────────────────────────────────────────────────────
local bIsGrounded     = true
local bIsMoving       = false
local bJumpedThisFrame = false

local GROUNDED_GRACE   = 0.12 
local fGroundedTimer   = GROUNDED_GRACE

-- Coyote time
local COYOTE_TIME     = 0.12
local fCoyoteTimer    = 0.0

-- Jump buffer
local JUMP_BUFFER     = 0.12
local fJumpBufferTimer = 0.0

-- Reference à la State Machine
local oSM = nil
local vecGravityDirection = fmath.vec3:new(0, -1, 0)

local function GetGravitySignY()
    return vecGravityDirection.y >= 0 and 1 or -1
end



-- ─── Mouvement ────────────────────────────────────────────────────────────────

self.MoveForward = function(icForward)
    -- print("FORWARD")
    if not oRB then return end
    bIsMoving = true
    oRB:ApplyLocalForceAtCenterOfMass(fmath.vec3:new(0, 0, -fMoveSpeed))
end

self.MoveBackward = function(icBackward)
    if not oRB then return end
    bIsMoving = true
    oRB:ApplyLocalForceAtCenterOfMass(fmath.vec3:new(0, 0, fMoveSpeed))
end

self.MoveLeft = function(icLeft)
    if not oRB then return end
    bIsMoving = true
    oRB:ApplyLocalForceAtCenterOfMass(fmath.vec3:new(-fMoveSpeed, 0, 0))
end

self.MoveRight = function(icRight)
    if not oRB then return end
    bIsMoving = true
    oRB:ApplyLocalForceAtCenterOfMass(fmath.vec3:new(fMoveSpeed, 0, 0))
end

self.DebugPos = function(icA)
    if not oRB then return end
    if not icA:IsPressed() then return end
    oRB:SetLocalPosition(fmath.vec3:new(-5,2,5))
end

-- ─── Saut ─────────────────────────────────────────────────────────────────────

self.Jump = function(icJump)
    if not oRB then return end
    if icJump:IsPressed() then
        fJumpBufferTimer = JUMP_BUFFER
    end
end

local function TryJump()
    if fCoyoteTimer <= 0 or fJumpBufferTimer <= 0 then return end -- CETTE LIGNE PERMET D'EMPECHER LES SAUTS SUCCESSIFS
    if fJumpBufferTimer <= 0 then return end

    local vel = oRB:GetLinearVelocity()

    oRB:SetLinearVelocity(fmath.vec3:new(vel.x, 0, vel.z))
    oRB:SetAngularVelocity(fmath.vec3:new(0, 0, 0))

    local vecJumpForce = fmath.vec3:new(
        -vecGravityDirection.x * fJumpForce,
        -vecGravityDirection.y * fJumpForce,
        -vecGravityDirection.z * fJumpForce
    )

    oRB:ApplyWorldForceAtCenterOfMass(vecJumpForce)

    fJumpBufferTimer = 0
    fCoyoteTimer     = 0
    bIsGrounded      = false
    bJumpedThisFrame = true

    print("JUMPING : " .. fJumpForce)

    if oSM then oSM:TransitionTo("JUMP") end
end

-- ─── Gravité custom ───────────────────────────────────────────────────────────

local function ApplyCustomGravity(oMass, dt)
    local gravity
    local vel = oRB:GetLinearVelocity()
    local fVerticalVel = vel.x * vecGravityDirection.x + vel.y * vecGravityDirection.y + vel.z * vecGravityDirection.z

    if bIsGrounded then
        return
    elseif fVerticalVel < 0 then
        gravity = GRAVITY_UP
    elseif fVerticalVel > 0.1 and not bJumpedThisFrame then
        gravity = GRAVITY_FALLING
    else
        gravity = GRAVITY_DOWN
    end

    local fGravityForce = gravity * oMass * dt
    local vecForce = fmath.vec3:new(
        vecGravityDirection.x * -fGravityForce,
        vecGravityDirection.y * -fGravityForce,
        vecGravityDirection.z * -fGravityForce
    )

    oRB:ApplyWorldForceAtCenterOfMass(vecForce)
end


-- ─── Update physique 

local bWasMovingLastFrame = false

function self:PhysicsUpdate(dt)
    if not oRB then return end
    local oMass = oRB:GetMass()
    fMoveSpeed = MOVE_SPEED * dt
    fJumpForce = JUMP_FORCE * dt

    -- grounded timer = time before falling (after running off an edge)
    if fGroundedTimer > 0 then
        fGroundedTimer = fGroundedTimer - dt
        if fGroundedTimer <= 0 then
            bIsGrounded = false
        end
    end

    -- Coyote time = time to jump while falling (after running off an edge)
    if bIsGrounded then
        fCoyoteTimer = COYOTE_TIME
    else
        fCoyoteTimer = fmath.Max(0, fCoyoteTimer - dt)
    end
    fJumpBufferTimer = fmath.Max(0, fJumpBufferTimer - dt)

    TryJump()
    ApplyCustomGravity(oMass, dt)

    bWasMovingLastFrame = bIsMoving
    bIsMoving           = false
    bJumpedThisFrame    = false
end


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
        -- si rien ici, le joueur reste "grounded" pendant GROUNDED_GRACE seconde(s)
    end
end


function self:IsMoving()
    return bWasMovingLastFrame  
end

function self:GetVerticalVelocity()
    if not oRB then return 0 end
    local vel = oRB:GetLinearVelocity()
    return vel.x * vecGravityDirection.x + vel.y * vecGravityDirection.y + vel.z * vecGravityDirection.z
end

function self:SetGravityDirection(vecNewGravityDirection)
    if not vecNewGravityDirection then
        return
    end

    local len = fmath.Sqrt(
        vecNewGravityDirection.x * vecNewGravityDirection.x +
        vecNewGravityDirection.y * vecNewGravityDirection.y +
        vecNewGravityDirection.z * vecNewGravityDirection.z
    )
    if len < 0.0001 then
        return
    end

    vecGravityDirection = fmath.vec3:new(
        vecNewGravityDirection.x / len,
        vecNewGravityDirection.y / len,
        vecNewGravityDirection.z / len
    )

    -- Legacy compatibility: some scripts still read oRB.gravity.
    oRB.gravity = GetGravitySignY()
end

function self:GetGravityDirection()
    return vecGravityDirection
end

function self:GetGravitySign()
    return GetGravitySignY()
end

-- ─── Injection de la State Machine 

function self:SetStateMachine(oNewSM)
    oSM = oNewSM
    print("StateMachine set in MovementComponent")
end

-- ─── Setup 

function self:SetMoveSpeed(iNewMoveSpeed)
    assert(type(iNewMoveSpeed) == "number", "MovementComponent: Valid number expected for move speed")
    fMoveSpeed = iNewMoveSpeed
end

function self:GetMoveSpeed() return fMoveSpeed end

function self:SetJumpForce(iNewJumpForce)
    assert(type(iNewJumpForce) == "number", "MovementComponent: Valid number expected for jump force")
    fJumpForce = iNewJumpForce
end

function self:GetJumpForce() return fJumpForce end

function self:Setup(oNewRigidBody, iNewMoveSpeed, iNewJumpForce)
    assert(oNewRigidBody ~= nil, "MovementComponent: Valid rigidbody must be provided")

    oRB        = oNewRigidBody
    MOVE_SPEED = iNewMoveSpeed or 10000
    JUMP_FORCE = iNewJumpForce or 800000
    self:SetGravityDirection(vecGravityDirection)

    print("MovementComponent Initialized")
end

