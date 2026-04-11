local iMoveSpeed = 1
local oCurrentEntity

function MoveForward(icForward)
    self:AddLocalZ(-iMoveSpeed)
end

function MoveBackward(icBackward)
    self:AddLocalZ(iMoveSpeed)
end

function MoveLeft(icLeft)
    self:AddLocalX(-iMoveSpeed)
end

function MoveRight(icRight)
    self:AddLocalX(iMoveSpeed)
end

function MoveDown(icDown)
    self:AddLocalY(-iMoveSpeed)
end

function MoveUp(icUp)
    self:AddLocalY(iMoveSpeed)
end

function SetCurrentEntity(ndHit)
    if oCurrentEntity == ndHit then
        return
    end

    oCurrentEntity = ndHit
end

function CheckInteraction()
    local hit = physics.Raycast(self:GetPosition(), self:GetLocalForward(), 100, 1)

    if not hit.node then
        return
    end

    local hitNode = hit.node
    local child = hitNode:FindChild("InteractReciever")

    if not child then
        print("element touche: "..hitNode:GetName())
        return
    end

    if child:CanInteract() then
        child:GetPrompt()
        SetCurrentEntity(child)
    else
        print("Cannot be interacted")
    end
end

function TryInteract()
    oCurrentEntity:Interact()
end



function OnInit()
    local actionmap = actionmap:new("DEFAULT_ACTION_MAP")

    actionmap:CreateAction("MOVE_FORWARD", 1, 90)
    actionmap:CreateAction("MOVE_BACKWARD", 1, 83)
    actionmap:CreateAction("MOVE_LEFT", 1, 81)
    actionmap:CreateAction("MOVE_RIGHT", 1, 68)
    actionmap:CreateAction("MOVE_DOWN", 1, 65)
    actionmap:CreateAction("MOVE_UP", 1, 69)

    actionmap:CreateAction("INTERACT", 1, 70)

    actionmap:GetAction("MOVE_FORWARD").Event = MoveForward
    actionmap:GetAction("MOVE_BACKWARD").Event = MoveBackward
    actionmap:GetAction("MOVE_LEFT").Event = MoveLeft
    actionmap:GetAction("MOVE_RIGHT").Event = MoveRight
    actionmap:GetAction("MOVE_DOWN").Event = MoveDown
    actionmap:GetAction("MOVE_UP").Event = MoveUp

    actionmap:GetAction("INTERACT").Event = TryInteract
end

function OnUpdate(dt)
    CheckInteraction()
end

function OnDestroy()

end