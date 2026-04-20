---@class noderigidbody
self = self

local acmPlayer
local oWindow
local oCameraRoot
local oCamera
local oMovementComponent
local oLookComponent
local oInteractEmitterComponent
local oStateMachineComponent

self.gravity = 1

local function InitializeRigidbody(iBodyType, iMass, bGravityEnabled, tAngularAxisLock, iLinearDamping, iAngularDamping)
    self:SetBodyType(iBodyType or 2)
    self:SetMass(iMass or 10)
    self:SetIsGravityEnabled(false)
    -- self:SetBounciness(0,0)

    local tAngularAxisLock = tAngularAxisLock or {}
    local x = type(tAngularAxisLock[1]) == "boolean" and tAngularAxisLock[1] or true
    local y = type(tAngularAxisLock[2]) == "boolean" and tAngularAxisLock[2] or false
    local z = type(tAngularAxisLock[3]) == "boolean" and tAngularAxisLock[3] or true
    self:LockAngularAxis(x, y, z)
    self:SetFrictionCoefficient(0, 100)
    self:SetLinearDamping(iLinearDamping or 1)
    self:SetAngularDamping(iAngularDamping or 10)
end

local function InitializeActionMap()
    acmPlayer = actionmap:new("PLAYER")
    acmPlayer:CreateAction("MOVE_FORWARD", 1, 90)
    acmPlayer:CreateAction("MOVE_BACKWARD", 1, 83)
    acmPlayer:CreateAction("MOVE_LEFT", 1, 81)
    acmPlayer:CreateAction("MOVE_RIGHT", 1, 68)
    acmPlayer:CreateAction("ROTATE_LEFT", 1, 65)
    acmPlayer:CreateAction("ROTATE_RIGHT", 1, 69)
    acmPlayer:CreateAction("JUMP", 1, 32)
    acmPlayer:CreateAction("LOOK", 3, 4080)
    acmPlayer:CreateAction("CHANGE_CURSOR_STATE", 1, 292)
    acmPlayer:CreateAction("INTERACT", 1, EventInput.KEY_F)
    acmPlayer:CreateAction("TEST", 1, EventInput.KEY_G)
end

-- self.TestSetPos =  function(icGkey)
--     print("------- Setting Pos")
--     self:SetLocalPosition(fmath.vec3:new(-5,2,5))
--     print("------- Pos Set")
-- end
-- function self.TestSetPos(icGkey)
--     print("------- Setting Pos")
--     self:SetLocalPosition(fmath.vec3:new(-5,2,5))
--     print("------- Pos Set")
-- end
local function BindActions(oMovementComponent, oLookComponent, oInteractEmitterComponent)
    if oMovementComponent then
        acmPlayer:GetAction("MOVE_FORWARD").Event  = oMovementComponent.MoveForward or
            function() print("PlayerBase: Move forward callback missing") end
        acmPlayer:GetAction("MOVE_BACKWARD").Event = oMovementComponent.MoveBackward or
            function() print("PlayerBase: Move backward callback missing") end
        acmPlayer:GetAction("MOVE_LEFT").Event     = oMovementComponent.MoveLeft or
            function() print("PlayerBase: Move left callback missing") end
        acmPlayer:GetAction("MOVE_RIGHT").Event    = oMovementComponent.MoveRight or
            function() print("PlayerBase: Move right callback missing") end
        acmPlayer:GetAction("ROTATE_LEFT").Event   = oMovementComponent.RotateLeft or
            function() print("PlayerBase: Rotate left callback missing") end
        acmPlayer:GetAction("ROTATE_RIGHT").Event  = oMovementComponent.RotateRight or
            function() print("PlayerBase: Rotate right callback missing") end
        acmPlayer:GetAction("JUMP").Event          = oMovementComponent.Jump or
            function() print("PlayerBase: Jump callback missing") end

        -- -- JUMP : buffer enregistré dans MovementComponent, saut effectif dans PhysicsUpdate
        -- acmPlayer:GetAction("JUMP").Event = function()
        --     oMovementComponent:Jump()
        -- end
    else
        print("PlayerBase: MovementComponent missing")
    end


    -- acmPlayer:GetAction("TEST_SETPOS").Event  = self.TestSetPos or
    --         function() print("BUG TEST SET POS") end

    if oLookComponent then
        acmPlayer:GetAction("LOOK").Event                = oLookComponent.HandleMouseLook or
            function() print("PlayerBase: Mouse move callback missing") end
        acmPlayer:GetAction("CHANGE_CURSOR_STATE").Event = oLookComponent.OnCursorStateChangePress or
            function() print("PlayerBase: Cursor state change callback missing") end
    else
        print("PlayerBase: LookComponent missing")
    end

    if oInteractEmitterComponent then
        acmPlayer:GetAction("INTERACT").Event = oInteractEmitterComponent.TryInteract or
            function() print("PlayerBase: Interact callback missing") end
    else
        print("PlayerBase: InteractComponent missing")
    end
end

function Interact()

end

function OnInit()
    print("PlayerBase Initializing")

    InitializeRigidbody()

    oWindow            = self:GetSceneTree():GetRoot()
    oCameraRoot        = self:GetNode("CameraRoot")
    oCamera            = self:GetNode("CameraRoot/Camera")

    -- MovementComponent
    oMovementComponent = self:GetNode("components/MovementComponent")
    if oMovementComponent ~= nil then
        oMovementComponent:Setup(self)
    end

    -- LookComponent
    oLookComponent = self:GetNode("components/LookComponent")
    if oLookComponent ~= nil then
        oLookComponent:Setup(self, oWindow, oCameraRoot)
    end

    -- InteractEmitterComponent
    oInteractEmitterComponent = self:GetNode("components/InteractEmitterComponent")
    if oInteractEmitterComponent ~= nil then
        oInteractEmitterComponent:Setup(self, 1.5)
    end

    -- StateMachineComponent
    oStateMachineComponent = self:GetNode("components/StateMachineComponent")
    if oStateMachineComponent ~= nil then
        oStateMachineComponent:Setup(self)
        if oMovementComponent ~= nil then
            oMovementComponent:SetStateMachine(oStateMachineComponent)
        end
    end

    InitializeActionMap()
    BindActions(oMovementComponent, oLookComponent, oInteractEmitterComponent)
end

local bRotating = false
local iGravityFieldRotationForce = 0.2
function OnUpdate(dt)
    -- Tick de la SM (transitions automatiques + OnUpdate des états)
    if oStateMachineComponent ~= nil then
        oStateMachineComponent:OnUpdate(dt)
    end
    OnPhysicsUpdate(dt)

    local vec = self:GetPosition()
    local vec2 = self:GetWorldPosition()

    -- print("Player LOCAL POS : {" .. vec.x .. ", ".. vec.y .. ", ".. vec.z .. "}")
    -- print("Player WORLD POS : {" .. vec2.x .. ", ".. vec2.y .. ", ".. vec2.z .. "}")
    -- print("_____")
    -- print("Player Forward : {"..self:GetLocalForward().x..";"..self:GetLocalForward().y..";"..self:GetLocalForward().z.."}")

    if oMovementComponent ~= nil then
        local hit = physics.Raycast(self:GetPosition(), fmath.vec3:new(0, -1, 0), 1.1)
        if hit then
            oMovementComponent:SetGrounded(true)
            -- print("_______ GROUNDED _________")
        else
            oMovementComponent:SetGrounded(false)
            -- print("+++++++ NOT GROUNDED +++++++++")
        end
    end
    if self.gravity > 0 then
        if self:GetPosition().y >= 10 then
            self.gravity = self.gravity * -1
            bRotating = true
        end
    else
        if self:GetPosition().y < 10 then
            self.gravity = self.gravity * -1
            bRotating = true
        end
    end
    if bRotating then
        -- OnGravityFieldChange()
        self:ApplyWorldTorque(fmath.vec3:new(10,0,0))
        print("Torque Applied")

        bRotating = false
        if self:GetLocalPitch() < 185 and self:GetLocalPitch() > 175
        then 
            self:SetWorldRotation(fmath.vec3:new(180))
            bRotating = false
            print("We have rotated !")
        end
    end
end

function OnPhysicsUpdate(dt)
    -- Gravité custom + jump buffer + coyote time
    if oMovementComponent ~= nil then
        oMovementComponent:PhysicsUpdate(dt)
    end
end

function OnCollisionEnter(oOther)
    if oMovementComponent ~= nil then
        oMovementComponent:SetGrounded(true)
    end
end

function OnCollisionExit(oOther)
    if oMovementComponent ~= nil then
        oMovementComponent:SetGrounded(false)
    end
end

-- local worldPitch = 0
-- local lerpFactor = 0
function OnGravityFieldChange()
            print(" --- ROTATING --- !" .. self:GetLocalPitch())
    -- self:ApplyWorldTorque(fmath.vec3:new(iGravityFieldRotationForce, 0, 0))
    self:AddLocalRotation(fmath.vec3:new(iGravityFieldRotationForce, 0, 0))
end
