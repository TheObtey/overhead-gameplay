local vecGravDirection = fmath.vec3:new(0, 1, 0)
local oCollider

local TriggerState = {
    ENTERED = 1,
    EXITED = 2
}

local function GetGravityReceiverFromBody(oBody)
    if not oBody then
        return nil
    end

    local oCompContainer = oBody:FindChild("components")
    if not oCompContainer then
        return nil
    end

    return oCompContainer:FindChild("GravityReceiverComponent")
end

local function OnBodyEntered(oRigidbody)
    if not oRigidbody then return end

    local oReceiver = GetGravityReceiverFromBody(oRigidbody)
    if oReceiver and oReceiver.ReceiveGravityFromField then
        oReceiver:ReceiveGravityFromField(self, vecGravDirection)
    end
end

local function OnBodyExited(oRigidbody)
    if not oRigidbody then return end

    local oReceiver = GetGravityReceiverFromBody(oRigidbody)
    if oReceiver and oReceiver.ReceiveGravityFromField then
        oReceiver:ClearGravityFromField(self)
    end
end

function OnInit()
    oCollider = self:GetNode("Collider")
    if oCollider ~= nil then
        oCollider = oCollider:As(NodeTypes.NODE_COLLIDER)
        oCollider:SetIsTrigger(true)
        oCollider:SetIsWorldQueryCollider(false)
        oCollider:SubscribeOnTrigger(function(_, oRigidbody, iState)
            if iState == TriggerState.ENTERED then
                OnBodyEntered(oRigidbody)
            elseif iState == TriggerState.EXITED then
                OnBodyExited(oRigidbody)
            end
        end)
    end
end

function OnUpdate(iDelta)
end