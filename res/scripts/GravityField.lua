local vecGravDirection = fmath.vec3:new(0, 1, 0)
local oGravDirComponent
local oCollider

function OnInit()
    -- oGravDirComponent = self:GetNode("components/GravDirComponent")

    -- if oGravDirComponent ~= nil then
    --     vecGravDirection = oGravDirComponent:GetDirection()
    -- end

    oCollider = self:FindChild("Triggerer"):As(NodeTypes.NODE_COLLIDER)
    if oCollider ~= nil then
        print("Iintializing oCollider with name : ".. oCollider:GetName())

        oCollider:SetIsTrigger(false) -- foncitonne pas dans le init => le collider est probablement pas existant au init du rigidbody

        print(oCollider:IsTrigger(), oCollider:IsSimulationCollider(), oCollider:IsWorldQueryCollider())

        -- Attach OnBodyEntered method of collider to a callback
        local id = oCollider:SubscribeOnTrigger(function(collider, rigidbody) -- Exemple Event 1
            print("TRIGGER avec : ".. rigidbody:GetName())
        end)
        oCollider:SubscribeOnTrigger(function(collider, rigidbody) -- Exemple Event 2
            print("ANOTHER ONE " .. rigidbody:GetName())
        end)
        
    end
end

function OnBodyEntered(oBody)
    -- Do shit
    print("YAA ZEBI")
end

local bColliderInitialized = false
local bColliderInitialized2 = false
function OnUpdate(iDelta)
 
    if not bColliderInitialized then
        oCollider:SetIsTrigger(true)
        oCollider:SetIsWorldQueryCollider(false)

        -- if bColliderInitialized2 then
        --     print(oCollider:IsTrigger(), oCollider:IsSimulationCollider(), oCollider:IsWorldQueryCollider())
        --     bColliderInitialized = true
        -- end
        -- bColliderInitialized2 = true
        bColliderInitialized = true
    end
end