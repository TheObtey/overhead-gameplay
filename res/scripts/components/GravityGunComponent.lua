local pEmitter
local pRootNode
local iMaxDistance
local oCurrentEntity
local oCompContainer
local oGGunTargetComp
local vecForward

-- Set the current node in front of the player
local function SetCurrentEntity(ndHit)
    if oCurrentEntity == ndHit then
        return
    end

    oCurrentEntity = ndHit
end
-- Do the raycast and analyze it
local function CheckInteraction(origin, forward, distance)
    pEmitter = pRootNode:FindChild("Player"):As(NodeTypes.NODE_RIGIDBODY)
    if not pEmitter then print("No emitter found with name 'Player'") return end
    vecForward = pEmitter:GetLocalForward()
    vecForward.z = vecForward.z * -1
    vecForward.x = vecForward.x * pEmitter.gravity
   
    local oHit = physics.Raycast(origin or pEmitter.Pos, forward or vecForward, distance or iMaxDistance)
    
    if not oHit then
        SetCurrentEntity(nil)
        return
    end
    
    if not oHit.node then
        SetCurrentEntity(nil)
        return
    end
    local raycasted = oHit.node:As(NodeTypes.NODE_RIGIDBODY)
    vecForward = raycasted:GetLocalForward()
    vecForward.z = vecForward.z * -1
   
    
    oCompContainer = oHit.node:FindChild("components")
    if not oCompContainer then return end
    oGGunTargetComp = oCompContainer:FindChild("GGunTargetComponent")

    if not oGGunTargetComp then
        SetCurrentEntity(oHit.node)
        return
    end

    if oGGunTargetComp:CanInteract() then
        if not oHit or not oHit.GetPrompt then
            print(oGGunTargetComp:GetPrompt())
        else
            print(oHit:GetPrompt())
        end
    end

    SetCurrentEntity(oHit.node)
end


local bUsingGGun = false
self.Use = function(icInteract)
    if icInteract:IsPressed() then
        -- local oPlayer = self:GetParent():GetParent():As(NodeTypes.NODE_RIGIDBODY)
        local oPlayer = pRootNode:FindChild("Player"):As(NodeTypes.NODE_RIGIDBODY)
        bUsingGGun = true
        print("")
        print("Problem name : " .. self:GetName())
        print("Parent name : " .. self:GetParent():GetName())
        if self:GetParent():GetParent() then
            print("Parent-parent name : " .. self:GetParent():GetParent():GetName())
            if self:GetParent():GetParent():GetParent() then
                print("Parent-parent-parent name : " .. self:GetParent():GetParent():GetParent():GetName())
                end
        end
        print("*******  STARTING Interaction  *******")

        print("Player name : " .. oPlayer:GetName())
        print("Player World pos : {" .. oPlayer:GetWorldPosition().x .. ", " .. oPlayer:GetWorldPosition().y .. ", " .. oPlayer:GetWorldPosition().z .. "}")
        print("Player Local pos : {" .. oPlayer:GetPosition().x .. ", " .. oPlayer:GetPosition().y .. ", " .. oPlayer:GetPosition().z .. "}")
        print("Player pos TEST : {" .. oPlayer.Pos.x .. ", " .. oPlayer.Pos.y .. ", " .. oPlayer.Pos.z .. "}")
        -- if oPlayer.refHeldObject then
        -- print("Player is hodling obj with name : "..oPlayer.refHeldObject:GetName())
        -- else
        -- print("Player is hodling obj with name : NONE")
        -- end
        print("____ TRY  GRABB _____")
        if oPlayer.refHeldObject == nil then
            CheckInteraction(_, _, 15)
        else
            oCurrentEntity = oPlayer.refHeldObject
        end

        if not oCurrentEntity then
            print("No grabbable in sight")
            return
        end

        oCompContainer = oCurrentEntity:FindChild("components")
        oGGunTargetComp = oCompContainer:FindChild("GGunTargetComponent")

        if not oGGunTargetComp then return end
        if oGGunTargetComp:CanInteract() then
            if oPlayer.bIsHoldingObject == 0 then
                oGGunTargetComp:GravityGunGrabb()
            else
                oGGunTargetComp:GravityGunThrow()
            end
        end
        print("Player name : " .. oPlayer:GetName())
        print("Player World pos : {" ..
        oPlayer:GetWorldPosition().x .. ", " .. oPlayer:GetWorldPosition().y .. ", " .. oPlayer:GetWorldPosition().z ..
        "}")
        print("Player Local pos : {" ..
        oPlayer:GetPosition().x .. ", " .. oPlayer:GetPosition().y .. ", " .. oPlayer:GetPosition().z .. "}")
        print("Player pos TEST : {" .. oPlayer.Pos.x .. ", " .. oPlayer.Pos.y .. ", " .. oPlayer.Pos.z .. "}")
        print("*******  ENDING Interaction  *******")
        print("")
    end
end

--Set up the component
function self:Setup(pNewEmitter, iNewMaxDistance)
    pEmitter = pNewEmitter
    iMaxDistance = iNewMaxDistance
    -- pRootNode = self:GetParent():GetParent():GetParent()
    -- pRootNode = self:GetParent()
    -- while pRootNode:GetParent() ~= nil do
    --     pRootNode = pRootNode:GetParent()
    -- end
    pRootNode = self:GetParent()
    while pRootNode:GetName() ~= "SceneRoot" do
        pRootNode = pRootNode:GetParent()
    end
end

function OnInit() end

function OnUpdate(dt) end
function OnDestroy() end