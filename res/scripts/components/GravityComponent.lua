local oRB
local pRootNode

local GRAVITY_UP      = -450.0   -- montée légère (saut)
local GRAVITY_DOWN    = -650.0   -- descente rapide
local GRAVITY_FALLING = -950.0   -- chute libre

local iMass = 1

local function ApplyCustomGravity(dt)
    if oRB == nil then
        print("oRB is nil stupid")
        return
    else
        -- print("It's okkkk : "..oRB:GetName())
    end
    if oRB:GetLinearVelocity() == nil then
        print("oRB:GetLinearVelocity() is nil stupid")
        return
    end
    local vel = oRB:GetLinearVelocity()
    local gravity

    -- if bIsGrounded then
    --     return
    -- else
        if vel.y > 0 then
        gravity = GRAVITY_UP
    elseif vel.y < -0.1 then
        gravity = GRAVITY_FALLING
    else
        gravity = GRAVITY_DOWN
    end

    local fGravityForce = gravity * iMass * dt

    oRB:ApplyWorldForceAtCenterOfMass(fmath.vec3:new(0, fGravityForce, 0))
end


function OnInit()
    oRB = self:GetParent()
    if oRB ~= nil then
        if oRB:GetName() == "components" then
            oRB = oRB:GetParent():As(NodeTypes.NODE_RIGIDBODY)
        else
            print("Components must be placed in 'components' Node, itself in ObjectNode")
        end
        pRootNode = oRB:GetParent()
        while pRootNode:GetName() ~= "SceneRoot" do
            pRootNode = pRootNode:GetParent()
        end
    end
end

function OnUpdate(fDelta)
    ApplyCustomGravity(fDelta)
end