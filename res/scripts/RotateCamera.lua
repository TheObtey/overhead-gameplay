local iMoveSpeed = 1.0

function MoveForward(icForward) print("move forward")
    self:AddLocalZ(-iMoveSpeed)

    print(self:GetWorldPosition().x, self:GetWorldPosition().y, self:GetWorldPosition().z)
    -- self:SetWorldPosition(fmath.vec3:new(0, 0, -iMoveSpeed))
    print(self:GetWorldPosition().x, self:GetWorldPosition().y, self:GetWorldPosition().z)
    print("\n\n")
end

function MoveBackward(icBackward) print("move backward")
    self:AddLocalZ(iMoveSpeed)

    print(self:GetWorldPosition().x, self:GetWorldPosition().y, self:GetWorldPosition().z)
    -- self:SetWorldPosition(fmath.vec3:new(0, 0, iMoveSpeed))
    print(self:GetWorldPosition().x, self:GetWorldPosition().y, self:GetWorldPosition().z)
    print("\n\n")
end

function MoveLeft(icLeft) print("move left")
    self:AddLocalX(-iMoveSpeed)

    print(self:GetWorldPosition().x, self:GetWorldPosition().y, self:GetWorldPosition().z)
    -- self:SetWorldPosition(fmath.vec3:new(-iMoveSpeed, 0, 0))
    print(self:GetWorldPosition().x, self:GetWorldPosition().y, self:GetWorldPosition().z)
    print("\n\n")
end

function MoveRight(icRight) print("move right")
    self:AddLocalX(iMoveSpeed)

    print(self:GetWorldPosition().x, self:GetWorldPosition().y, self:GetWorldPosition().z)
    -- self:SetWorldPosition(fmath.vec3:new(iMoveSpeed, 0, 0))
    print(self:GetWorldPosition().x, self:GetWorldPosition().y, self:GetWorldPosition().z)
    print("\n\n")
end

function MoveDown(icDown)
    -- self:AddLocalY(-iMoveSpeed)
end

function MoveUp(icUp)
    -- self:AddLocalY(iMoveSpeed)
end

function OnInit() print("big shit")
    local actionmap = actionmap:new("DEFAULT_ACTION_MAP")

    actionmap:CreateAction("MOVE_FORWARD", 1, 90)
    actionmap:CreateAction("MOVE_BACKWARD", 1, 83)
    actionmap:CreateAction("MOVE_LEFT", 1, 81)
    actionmap:CreateAction("MOVE_RIGHT", 1, 68)
    actionmap:CreateAction("MOVE_DOWN", 1, 65)
    actionmap:CreateAction("MOVE_UP", 1, 69)

    actionmap:GetAction("MOVE_FORWARD").Event = MoveForward
    actionmap:GetAction("MOVE_BACKWARD").Event = MoveBackward
    actionmap:GetAction("MOVE_LEFT").Event = MoveLeft
    actionmap:GetAction("MOVE_RIGHT").Event = MoveRight
    actionmap:GetAction("MOVE_DOWN").Event = MoveDown
    actionmap:GetAction("MOVE_UP").Event = MoveUp
end

function OnUpdate(dt)

end

function OnDestroy()

end
