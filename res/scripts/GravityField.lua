local vecGravDirection = fmath.vec3:new(0, 1, 0)
local oCollider

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

local function OnTrigger(collider, rigidbody)
    if not rigidbody then return end

    local oReceiver = GetGravityReceiverFromBody(rigidbody)
    if oReceiver and oReceiver.ReceiveGravityFromField then
        oReceiver:ReceiveGravityFromField(self, vecGravDirection)
    end
end

function OnInit()
    oCollider = self:GetNode("Collider")
    if oCollider ~= nil then
        oCollider = oCollider:As(NodeTypes.NODE_COLLIDER)
        oCollider:SetIsTrigger(true)
        oCollider:SetIsWorldQueryCollider(false)
        oCollider:SubscribeOnTrigger(function(collider, rigidbody)
            OnTrigger(collider, rigidbody)
        end)
    end
end

function OnUpdate(iDelta)
end