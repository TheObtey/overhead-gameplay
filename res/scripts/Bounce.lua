---@type NodeRigidBody

local iJumpBoost = 15
local oCollider

local function LetsBounce(rb)
    local vecBounce = vec3:new(0, iJumpBoost, 0)
    rb:ApplyLocalForceAtCenterOfMass(vecBounce)
end

function OnInit()
    oCollider = self:FindChild("Collider"):As(NodeTypes.NODE_COLLIDER)
    local id = oCollider:SubscribeOnContact(function(collider, rigidbody, state) -- Exemple Event 1
        LetsBounce(rigidbody)
    end)
end

function OnUpdate(dt)
end

function OnDestroy()
end
