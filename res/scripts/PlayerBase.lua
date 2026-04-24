-- ---@class noderigidbody
-- self = self

local acmPlayer
local oWindow
local oCameraRoot
local oCamera
local oMovementComponent
local oLookComponent
local oInteractEmitterComponent
local oStateMachineComponent
local oGravityGunComponent

local bRotating = false
self.Pos = fmath.vec3:new()

self.gravity = 1 -- doit devenir un vecteur & component

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
    -- self:SetFrictionCoefficient(0, 1)
    self:SetLinearDamping(iLinearDamping or 0.5)
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
    acmPlayer:CreateAction("TEST", 1, EventInput.KEY_C)
    acmPlayer:CreateAction("GRAVITY_GUN", 1, EventInput.KEY_G)
    acmPlayer:CreateAction("DEBUG_POS", 1, EventInput.KEY_A)
end

local function BindActions(oMovementComponent, oLookComponent, oInteractEmitterComponent, oGravityGunComponent)
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
        acmPlayer:GetAction("DEBUG_POS").Event     = oMovementComponent.DebugPos or
            function() print("PlayerBase: DEBUG_POS callback missing") end
        acmPlayer:GetAction("JUMP").Event          = oMovementComponent.Jump or
            function() print("PlayerBase: Jump callback missing") end
    else
        print("PlayerBase: MovementComponent missing")
    end

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

    if oGravityGunComponent then
        acmPlayer:GetAction("GRAVITY_GUN").Event = oGravityGunComponent.Use or
            function() print("PlayerBase: Gravity Gun callback Try grabb missing") end
            print("PlayerBase: GravityGunComponent SUCCESS")
    else
        print("PlayerBase: GravityGunComponent missing")
    end
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

    -- GravityGunComponent (MOVE TO GRAVITY GUN OBJECT)
    oGravityGunComponent = self:GetNode("components/GravityGunComponent")
    if oGravityGunComponent ~= nil then
        oGravityGunComponent:Setup(self, 1.5)
    end

    InitializeActionMap()
    BindActions(oMovementComponent, oLookComponent, oInteractEmitterComponent, oGravityGunComponent)
end

local count = 0
function OnUpdate(dt)
    -- Tick de la SM (transitions automatiques + OnUpdate des états)
    if oStateMachineComponent ~= nil then
        oStateMachineComponent:OnUpdate(dt)
    end
    OnPhysicsUpdate(dt)
    self.Pos = self:GetPosition()

    
    -- DEBUG Pos toutes les 4000 frames
    -- local vec = self:GetPosition()
    -- local vec2 = self:GetWorldPosition()
    -- self.Pos = self:GetPosition()
    -- count = count + 1
    -- if count > 4000 then
    --     print("_____")
    --     print(self:GetName() .. " LOCAL POS : {" .. vec.x .. ", " .. vec.y .. ", " .. vec.z .. "}")
    --     print(self:GetName() .. " WORLD POS : {" .. vec2.x .. ", " .. vec2.y .. ", " .. vec2.z .. "}")
    --     print("_____")
    -- -- print("Player Forward : {"..self:GetLocalForward().x..";"..self:GetLocalForward().y..";"..self:GetLocalForward().z.."}")
    -- count = 0
    -- end

    CheckIfGrounded()
    CheckGravityField(dt)
    if bRotating then
        OnGravityFieldChange(dt)
    end
end

function OnPhysicsUpdate(dt)
    -- Gravité custom + jump buffer + coyote time
    if oMovementComponent ~= nil then
        oMovementComponent:PhysicsUpdate(dt)
    end
end

function CheckIfGrounded()
    if oMovementComponent ~= nil then
        local hit = physics.Raycast(self:GetPosition(), fmath.vec3:new(0, -1 * self.gravity, 0), 1.001)
        if hit then
            oMovementComponent:SetGrounded(true)
            -- print("_______ GROUNDED _________")
        else
            oMovementComponent:SetGrounded(false)
            -- print("+++++++ NOT GROUNDED +++++++++")
        end
    end
end

function CheckGravityField(dt)
    if self.gravity > 0 then
        if self:GetPosition().y >= 10 then
            self.gravity = self.gravity * -1
            bRotating = true
            self.bIsRotating = false
        end
    else
        if self:GetPosition().y < 10 then
            self.gravity = self.gravity * -1
            bRotating = true
            self.bIsRotating = false
        end
    end
end

self.bIsRotating = false
local worldRoll = 0
local lerpFactor = 0
local bTest = false
function OnGravityFieldChange(dt)
    -- self:LockAngularAxis(true, true, true)
    bTest = not bTest
    -- if not bTest then return end
    local roll = self:GetLocalRoll()
    print(" --- ROTATING --- !" .. roll)

    self:AddLocalRotation(fmath.vec3:new(0, 0, dt * 1.5 * fmath.Pi))

    worldRoll = worldRoll + dt * 1.5 * fmath.Pi
    print("Player World Roll = " .. worldRoll)

    if worldRoll >= 3.13 or worldRoll <= -3.13
    then
        if self.gravity == -1 then
            self:SetLocalRotationDeg(fmath.vec3:new(0, 0, 180))
        else
            if self.gravity == 1 then
                self:SetLocalRotationDeg(fmath.vec3:new(0, 0, 0))
            end
        end
        -- self:LockAngularAxis(true, false, true)
        print("We have rotated !")
        worldRoll = 0
        bRotating = false
        self.bIsRotating = false
    end
end
