
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



-- ─── Mouvement ────────────────────────────────────────────────────────────────
local iForwardcount = 0
self.MoveForward = function(icForward)
    if not oRB and not icForward.IsHold() then return end
    -- bIsMoving = true
    -- oRB:ApplyLocalForceAtCenterOfMass(fmath.vec3:new(0, 0, -fMoveSpeed))
    if icForward:IsHold() then
        iForwardcount =  iForwardcount + 1
        bIsMoving = true
        oRB:ApplyLocalForceAtCenterOfMass(fmath.vec3:new(0, 0, -fMoveSpeed))
    else
        if icForward:IsReleased() then
            local vel = oRB:GetLinearVelocity()
            oRB:SetLinearVelocity(fmath.vec3:new(vel.x, vel.y, vel.z/2))
            -- oRB:ApplyLocalForceAtCenterOfMass(fmath.vec3:new(0, 0, fMoveSpeed*iForwardcount/2))
            iForwardcount = 0
        end
    end
end

local iBackwardcount = 0
self.MoveBackward = function(icBackward)
    if not oRB and not icBackward.IsHold() then return end
    -- bIsMoving = true
    -- oRB:ApplyLocalForceAtCenterOfMass(fmath.vec3:new(0, 0, fMoveSpeed))
    if icBackward:IsHold() then
        bIsMoving = true
        iBackwardcount = iBackwardcount + 1
        oRB:ApplyLocalForceAtCenterOfMass(fmath.vec3:new(0, 0, fMoveSpeed))
    else
        if icBackward:IsReleased() then
            local vel = oRB:GetLinearVelocity()
            oRB:SetLinearVelocity(fmath.vec3:new(vel.x, vel.y, vel.z/2))
            -- oRB:ApplyLocalForceAtCenterOfMass(fmath.vec3:new(0, 0, -fMoveSpeed*iBackwardcount/2))
            iBackwardcount = 0
        end
    end
end

local iLeftcount = 0
self.MoveLeft = function(icLeft)
    if not oRB and not icLeft.IsHold() then return end
    -- bIsMoving = true
    -- oRB:ApplyLocalForceAtCenterOfMass(fmath.vec3:new(-fMoveSpeed, 0, 0))
    if icLeft:IsHold() then
        bIsMoving = true
        oRB:ApplyLocalForceAtCenterOfMass(fmath.vec3:new(-fMoveSpeed, 0, 0))
    else
        if icLeft:IsReleased() then
            local vel = oRB:GetLinearVelocity()
            oRB:SetLinearVelocity(fmath.vec3:new(vel.x/2, vel.y, vel.z))
            -- oRB:ApplyLocalForceAtCenterOfMass(fmath.vec3:new(fMoveSpeed, 0, 0))
        end
    end
end

local iRightcount = 0
self.MoveRight = function(icRight)
    if not oRB and not icRight.IsHold() then return end
    -- bIsMoving = true
    -- oRB:ApplyLocalForceAtCenterOfMass(fmath.vec3:new(fMoveSpeed, 0, 0))
    if icRight:IsHold() then
        bIsMoving = true
        oRB:ApplyLocalForceAtCenterOfMass(fmath.vec3:new(fMoveSpeed, 0, 0))
    else
        if icRight:IsReleased() then
            local vel = oRB:GetLinearVelocity()
            oRB:SetLinearVelocity(fmath.vec3:new(vel.x/2, vel.y, vel.z))
            -- oRB:ApplyLocalForceAtCenterOfMass(fmath.vec3:new(-fMoveSpeed, 0, 0))
        end
    end
end
self.DebugPos = function(icA)
    if not oRB and not icA.IsHold() then return end
    if icA:IsHold() then
    oRB:SetLocalPosition(fmath.vec3:new(-5,2,5))
    end
end

-- ─── Saut ─────────────────────────────────────────────────────────────────────

self.Jump = function(icJump)
    if not oRB and not icJump.IsHold() then return end
    fJumpBufferTimer = JUMP_BUFFER
end

local function TryJump()
    if fJumpBufferTimer > 0 then -- CETTE LIGNE PERMET LES SAUTS SUCCESSIFS 
    -- if fCoyoteTimer > 0 and fJumpBufferTimer > 0 then 
        local vel = oRB:GetLinearVelocity()
        oRB:SetLinearVelocity(fmath.vec3:new(vel.x, 0, vel.z))
        oRB:SetAngularVelocity(fmath.vec3:new(0, 0, 0))
        oRB:ApplyLocalForceAtCenterOfMass(fmath.vec3:new(0, fJumpForce, 0))

        fJumpBufferTimer = 0
        fCoyoteTimer     = 0
        bIsGrounded      = false
        bJumpedThisFrame = true
        print("JUMPING : " .. fJumpForce)
        if oSM then oSM:TransitionTo("JUMP") end
    end
end

-- ─── Gravité custom ───────────────────────────────────────────────────────────

local function ApplyCustomGravity(oMass, dt)
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
    local fGravityForce = gravity * oMass * dt * oRB.gravity

    oRB:ApplyWorldForceAtCenterOfMass(fmath.vec3:new(0, fGravityForce, 0))
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
    MOVE_SPEED = iNewMoveSpeed or 8000
    JUMP_FORCE = iNewJumpForce or 600000

    print("MovementComponent Initialized")
end

