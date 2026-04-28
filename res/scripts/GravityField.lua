local oCollider
local StateType = {
    ENTERED = 0,
    EXITED = 2
}

local function OnBodyEntered(oBody)
    if not oBody then return end

    if oBody.gravity then
        oBody.gravity = -1
        oBody.bRotating = true
        oBody.bIsRotating = false
    end

    if oBody.SetGravityDirection then
        oBody:SetGravityDirection(-1)
    end
end

local function OnBodyExited(oBody)
    if not oBody then return end

    if oBody.gravity then
        oBody.gravity = 1
        oBody.bRotating = true
        oBody.bIsRotating = false
    end

    if oBody.SetGravityDirection then
        oBody:SetGravityDirection(1)
    end
end

function OnInit()
    oCollider = self:GetNode("Collider")
    if not oCollider then return end

    oCollider = oCollider:As(NodeTypes.NODE_COLLIDER)

    print("Iintializing oCollider with name : ".. oCollider:GetName())

    -- Attach OnBodyEntered method of collider to a callback
    oCollider:SubscribeOnTrigger(function(_, oRigidbody, iState)
        if iState == StateType.ENTERED then
            OnBodyEntered(oRigidbody)
        elseif iState == StateType.EXITED then
            OnBodyExited(oRigidbody)
        end
    end)
end

local bColliderInitialized = false
function OnUpdate(iDelta)
    if not bColliderInitialized and oCollider ~= nil then
        oCollider:SetIsTrigger(true)
        oCollider:SetIsWorldQueryCollider(false)

        bColliderInitialized = true
    end
end