local oOwnerBody
local oMovementComponent

function self:Setup(oNewOwnerBody)
    oOwnerBody = oNewOwnerBody
    if not oOwnerBody then
        return
    end

    local oCompContainer = oOwnerBody:FindChild("components")
    if not oCompContainer then
        return
    end

    oMovementComponent = oCompContainer:FindChild("MovementComponent")
end

function self:ApplyGravityDirection(vecGravityDirection)
    if not oMovementComponent then
        return
    end

    if oMovementComponent.SetGravityDirection then
        oMovementComponent:SetGravityDirection(vecGravityDirection)
    end
end
