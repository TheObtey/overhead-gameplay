local iMoveSpeed = 100.0
local parent

local ActionMap

---@class noderigidbody
self = self

function MoveForward(icForward)
    print("move forward")
    self:ApplyLocalForceAtCenterOfMass(fmath.vec3:new(0, 0, -iMoveSpeed))
    -- self:SetLinearVelocity(fmath.vec3:new(0, 0, -iMoveSpeed)) // CA CEST PAREIL QUE ApplyWorldForce...()

    print(parent:GetPosition().x, parent:GetPosition().y, parent:GetPosition().z)
    print("\n\n")
end

function MoveBackward(icBackward) print("move backward")
    self:ApplyLocalForceAtCenterOfMass(fmath.vec3:new(0, 0, iMoveSpeed))
    -- self:SetLinearVelocity(fmath.vec3:new(0, 0, iMoveSpeed))

    print(parent:GetPosition().x, parent:GetPosition().y, parent:GetPosition().z)
    print("\n\n")
end

function MoveLeft(icLeft) print("move left")
    self:ApplyLocalForceAtCenterOfMass(fmath.vec3:new(-iMoveSpeed, 0, 0))
    -- self:SetLinearVelocity(fmath.vec3:new(-iMoveSpeed, 0, 0))

    print(parent:GetPosition().x, parent:GetPosition().y, parent:GetPosition().z)
    print("\n\n")
end

function MoveRight(icRight) print("move right")
    self:ApplyLocalForceAtCenterOfMass(fmath.vec3:new(iMoveSpeed, 0, 0))
    -- self:SetLinearVelocity(fmath.vec3:new(iMoveSpeed, 0, 0))

    print(parent:GetPosition().x, parent:GetPosition().y, parent:GetPosition().z)
    print("\n\n")
end

function MoveDown(icDown)
    self:ApplyLocalForceAtCenterOfMass(fmath.vec3:new(0, -iMoveSpeed, 0))
    -- self:SetLinearVelocity(fmath.vec3:new(0, -iMoveSpeed, 0))
end

function MoveUp(icUp)
    if icUp:IsPressed() then
        self:ApplyLocalForceAtCenterOfMass(fmath.vec3:new(0, iMoveSpeed * 30, 0))
    end
    -- self:SetLinearVelocity(fmath.vec3:new(0, iMoveSpeed*2, 0))
end

function RotateRight(icRight)
    self:ApplyWorldTorque(fmath.vec3:new(0, iMoveSpeed*3, 0))
end

function RotateLeft(icLeft)
    print("rota")
    self:ApplyWorldTorque(fmath.vec3:new(0, -iMoveSpeed*3, 0))

end

function OnInit()
    ActionMap = actionmap:new("DEFAULT_ACTION_MAP")

    print("Begin RB INIT ")
    self:SetBodyType(2)
    self:SetMass(10)
    self:SetIsGravityEnabled(true)
    self:LockAngularAxis(true,false,true)
    self:SetLinearDamping(1)
    self:SetAngularDamping(0.5)
    parent = self:GetNode3DParent()


    ActionMap:CreateAction("MOVE_FORWARD", 1, 90)
    ActionMap:CreateAction("MOVE_BACKWARD", 1, 83)
    ActionMap:CreateAction("MOVE_LEFT", 1, 81)
    ActionMap:CreateAction("MOVE_RIGHT", 1, 68)
    ActionMap:CreateAction("MOVE_DOWN", 1, 65)
    ActionMap:CreateAction("MOVE_UP", ControlType.BUTTON, EventInput.KEY_SPACE)

    ActionMap:CreateAction("ROTATE_RIGHT", 1, 262)
    ActionMap:CreateAction("ROTATE_LEFT", 1, 263)


    ActionMap:GetAction("MOVE_FORWARD").Event = MoveForward
    ActionMap:GetAction("MOVE_BACKWARD").Event = MoveBackward
    ActionMap:GetAction("MOVE_LEFT").Event = MoveLeft
    ActionMap:GetAction("MOVE_RIGHT").Event = MoveRight
    ActionMap:GetAction("MOVE_DOWN").Event = MoveDown
    ActionMap:GetAction("MOVE_UP").Event = MoveUp

    ActionMap:GetAction("ROTATE_RIGHT").Event = RotateRight
    ActionMap:GetAction("ROTATE_LEFT").Event = RotateLeft
end

function OnUpdate(dt)

end

function OnDestroy()

end
