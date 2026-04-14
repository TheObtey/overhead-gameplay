local iMoveSpeed = 3000.0

local oParent

local oPlayerRB
local oCameraRoot
local oCamera

local iMouseSensitivity = 0.002
local iMinPitch = -89.0
local iMaxPitch = 89.0
local iPitch = 0.0

function MoveForward(icForward) print("move forward")
    self:ApplyLocalForceAtCenterOfMass(fmath.vec3:new(0, 0, -iMoveSpeed))

    -- self:SetLinearVelocity(fmath.vec3:new(0, 0, -iMoveSpeed)) // CA CEST PAREIL QUE ApplyWorldForce...()

    print(oParent:GetPosition().x, oParent:GetPosition().y, oParent:GetPosition().z)
    print("\n\n")
end

function MoveBackward(icBackward) print("move backward")
    self:ApplyLocalForceAtCenterOfMass(fmath.vec3:new(0, 0, iMoveSpeed))
    -- self:SetLinearVelocity(fmath.vec3:new(0, 0, iMoveSpeed))

    print(oParent:GetPosition().x, oParent:GetPosition().y, oParent:GetPosition().z)
    print("\n\n")
end

function MoveLeft(icLeft) print("move left")
    self:ApplyLocalForceAtCenterOfMass(fmath.vec3:new(-iMoveSpeed, 0, 0))
    -- self:SetLinearVelocity(fmath.vec3:new(-iMoveSpeed, 0, 0))

    print(oParent:GetPosition().x, oParent:GetPosition().y, oParent:GetPosition().z)
    print("\n\n")
end

function MoveRight(icRight) print("move right")
    self:ApplyLocalForceAtCenterOfMass(fmath.vec3:new(iMoveSpeed, 0, 0))
    -- self:SetLinearVelocity(fmath.vec3:new(iMoveSpeed, 0, 0))

    print(oParent:GetPosition().x, oParent:GetPosition().y, oParent:GetPosition().z)
    print("\n\n")
end

function MoveDown(icDown)
    self:ApplyLocalForceAtCenterOfMass(fmath.vec3:new(0, -iMoveSpeed, 0))
    -- self:SetLinearVelocity(fmath.vec3:new(0, -iMoveSpeed, 0))
end

function MoveUp(icUp)
    self:ApplyLocalForceAtCenterOfMass(fmath.vec3:new(0, iMoveSpeed, 0))
    -- self:SetLinearVelocity(fmath.vec3:new(0, iMoveSpeed*2, 0))
end

function RotateRight(icRight)
    self:ApplyWorldTorque(fmath.vec3:new(0, iMoveSpeed*3, 0))
end

function RotateLeft(icLeft)
    print("rota")
    self:ApplyWorldTorque(fmath.vec3:new(0, -iMoveSpeed*3, 0))

end

function OnMouseMoveCallback(control)
    local vecMouse = icontrol.ReadAsVec2(control)
    print(vecMouse.x, vecMouse.y)

    if not oPlayerRB or not oCameraRoot or not oCamera then return end

    local vecPlayerUp = oPlayerRB:GetLocalUp()

    oPlayerRB:AddLocalPitch(-vecMouse.x * iMouseSensitivity)

    -- iPitch = iPitch - vecMouse.y * iMouseSensitivity
    -- iPitch = fmath.Clamp(iPitch, fmath.Rad(fmath.Deg(iMinPitch)), fmath.Rad(fmath.Deg(iMaxPitch)))

    -- oCameraRoot:AddLocalPitch(iPitch)
end

local function SetupCamera(oNewRB, oNewCameraRoot, oNewCamera)
    oPlayerRB = oNewRB
    oCameraRoot = oNewCameraRoot
    oCamera = oNewCamera
end

function OnInit() 
    local actionmap = actionmap:new("DEFAULT_ACTION_MAP")

    print("Begin RB INIT ")
    self:SetBodyType(2)
    self:SetMass(10)
    self:SetIsGravityEnabled(true)
    self:LockAngularAxis(true,false,true)
    self:SetLinearDamping(1)
    self:SetAngularDamping(0.5)
    oParent = self:GetNode3DParent()

    oCameraRoot = oParent:GetNode("CameraRoot")
    oCamera = oParent:GetNode("CameraRoot/Camera")

    SetupCamera(self, oCameraRoot, oCamera)

    actionmap:CreateAction("MOVE_FORWARD", 1, 90)
    actionmap:CreateAction("MOVE_BACKWARD", 1, 83)
    actionmap:CreateAction("MOVE_LEFT", 1, 81)
    actionmap:CreateAction("MOVE_RIGHT", 1, 68)
    actionmap:CreateAction("MOVE_DOWN", 1, 65)
    actionmap:CreateAction("MOVE_UP", 1, 69)

    actionmap:CreateAction("ROTATE_RIGHT", 1, 262)
    actionmap:CreateAction("ROTATE_LEFT", 1, 263)

    actionmap:CreateAction("LOOK", 3, 4080)

    actionmap:GetAction("MOVE_FORWARD").Event = MoveForward
    actionmap:GetAction("MOVE_BACKWARD").Event = MoveBackward
    actionmap:GetAction("MOVE_LEFT").Event = MoveLeft
    actionmap:GetAction("MOVE_RIGHT").Event = MoveRight
    actionmap:GetAction("MOVE_DOWN").Event = MoveDown
    actionmap:GetAction("MOVE_UP").Event = MoveUp

    actionmap:GetAction("ROTATE_RIGHT").Event = RotateRight
    actionmap:GetAction("ROTATE_LEFT").Event = RotateLeft

    actionmap:GetAction("LOOK").Event = OnMouseMoveCallback

end

function OnUpdate(dt)

end

function OnDestroy()

end
