local iMoveSpeed = 30000.0
local parent

function MoveForward(icForward) print("move forward")
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

function OnInit() 
    local actionmap = actionmap:new("DEFAULT_ACTION_MAP")
    
    print("Begin RB INIT ")
    self:SetBodyType(2)
    self:SetMass(10)
    self:SetIsGravityEnabled(true)
    self:LockAngularAxis(true,false,true)
    self:SetLinearDamping(1)
    self:SetAngularDamping(0.5)
    parent = self:GetNode3DParent()
    
    
    actionmap:CreateAction("MOVE_FORWARD", 1, 90)
    actionmap:CreateAction("MOVE_BACKWARD", 1, 83)
    actionmap:CreateAction("MOVE_LEFT", 1, 81)
    actionmap:CreateAction("MOVE_RIGHT", 1, 68)
    actionmap:CreateAction("MOVE_DOWN", 1, 65)
    actionmap:CreateAction("MOVE_UP", 1, 69)
    
    actionmap:CreateAction("ROTATE_RIGHT", 1, 262)
    actionmap:CreateAction("ROTATE_LEFT", 1, 263)
    
    
    actionmap:GetAction("MOVE_FORWARD").Event = MoveForward
    actionmap:GetAction("MOVE_BACKWARD").Event = MoveBackward
    actionmap:GetAction("MOVE_LEFT").Event = MoveLeft
    actionmap:GetAction("MOVE_RIGHT").Event = MoveRight
    actionmap:GetAction("MOVE_DOWN").Event = MoveDown
    actionmap:GetAction("MOVE_UP").Event = MoveUp
    
    actionmap:GetAction("ROTATE_RIGHT").Event = RotateRight
    actionmap:GetAction("ROTATE_LEFT").Event = RotateLeft
end

function OnUpdate(dt)
    
end

function OnDestroy()
    
end