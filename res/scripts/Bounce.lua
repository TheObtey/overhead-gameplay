---@type NodeRigidBody

local iJumpBoost = 150
local oCollider

local function LetsBounce(rb)
    local vecBounce = fmath.vec3:new(0, iJumpBoost, 0)
    rb:ApplyWorldForceAtCenterOfMass(vecBounce)
end

function OnInit()
    oCollider = self:FindChild("Collider"):As(NodeTypes.NODE_COLLIDER)

    oCollider:SetBounciness(1)
    local id = oCollider:SubscribeOnContact(function(collider, rigidbody, state)
        LetsBounce(rigidbody)
    end)
end

function OnUpdate(dt)
end

function OnDestroy()
end
