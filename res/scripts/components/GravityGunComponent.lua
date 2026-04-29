local oPlayer
local pRootNode
local iMaxDistance
local oCurrentEntity
local oCompContainer
local oGGunTargetComp
local vecForward
local iCurDT

self.refHeldObject = nil

self.bIsHoldingObject = 0

self.SetHoldingObj = function(value)
    self.bIsHoldingObject = value
end
self.GetHoldingObj = function()
    return self.bIsHoldingObject
end

-- Set the current node in front of the player
local function SetCurrentEntity(ndHit)
    if oCurrentEntity == ndHit then
        return
    end

    oCurrentEntity = ndHit
end
-- Do the raycast and analyze it
local function CheckInteraction(origin, forward, distance)
    if not oPlayer then print("No emitter found with name 'Player'") return else print("Emitter in CheckInteraction GGun OK") end
    local oCameraRoot = oPlayer:FindChild("CameraRoot"):As(NodeTypes.NODE3D)
    local oCamera = oCameraRoot:FindChild("Camera"):As(NodeTypes.NODE_CAMERA)
    if not oCamera then print ("PB CAM GRAVITYGUN COMPONENT") return end
    vecForward = oPlayer:GetLocalForward()
    vecForward.y = oCameraRoot:GetLocalForward().y
    -- vecForward = oCamera:GetLocalForward()
    -- vecForward = oCamera.VecForward
    vecForward.z = vecForward.z * -1
    print("VecForward cam = ".. vecForward.x..", ".. vecForward.y..", ".. vecForward.z)
    -- vecForward.x = vecForward.x * oPlayer.gravity
    
    local oHit = physics.Raycast(origin or oPlayer.Pos, forward or vecForward, distance or iMaxDistance)
    
    if not oHit then
        SetCurrentEntity(nil)
        return
    end
    
    if not oHit.node then
        SetCurrentEntity(nil)
        return
    end
    local raycasted = oHit.node
    vecForward = raycasted:GetLocalForward()
    vecForward.z = vecForward.z * -1

    -- print("oCompContainer lookup...")
    -- print("oHitNode = ".. tostring(oHit.node))
    -- print("oHitNode name = ".. oHit.node:GetName())
    oCompContainer = oHit.node:FindChild("components")
    if not oCompContainer then print("oCompContainer NOT FOUND ")return end
    -- print("oCompContainer : " .. oCompContainer:GetName())
    oGGunTargetComp = oCompContainer:FindChild("GGunTargetComponent")

    -- oCompContainer = oHit.node:FindChild("components")
    -- oGGunTargetComp = oCompContainer:FindChild("GGunTargetComponent")

    if not oGGunTargetComp then
        SetCurrentEntity(oHit.node)
        print("not oGGunTargetComp")
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
local iThrowForce = 6000
local iCount = 0
self.Use = function(icInteract)
    if icInteract:IsHold() then
        iCount = iCount + iCurDT
        if iCount >= 2 then iCount = 2 end
    end
    if icInteract:IsReleased() then
        local oPlayer = pRootNode:FindChild("Player"):As(NodeTypes.NODE_RIGIDBODY)
        bUsingGGun = true
        -- DEBUG
        -- print("")
        -- print("Problem name : " .. self:GetName())
        -- print("Parent name : " .. self:GetParent():GetName())
        -- if self:GetParent():GetParent() then
        --     print("Parent-parent name : " .. self:GetParent():GetParent():GetName())
        --     if self:GetParent():GetParent():GetParent() then
        --         print("Parent-parent-parent name : " .. self:GetParent():GetParent():GetParent():GetName())
        --         end
        -- end
        -- print("*******  STARTING Interaction  *******")
        -- print("Player name : " .. oPlayer:GetName())
        -- print("Player World pos : {" .. oPlayer:GetWorldPosition().x .. ", " .. oPlayer:GetWorldPosition().y .. ", " .. oPlayer:GetWorldPosition().z .. "}")
        -- print("Player Local pos : {" .. oPlayer:GetPosition().x .. ", " .. oPlayer:GetPosition().y .. ", " .. oPlayer:GetPosition().z .. "}")
        -- print("Player pos TEST : {" .. oPlayer.Pos.x .. ", " .. oPlayer.Pos.y .. ", " .. oPlayer.Pos.z .. "}")
        -- if oPlayer.refHeldObject then
        -- print("Player is hodling obj with name : "..oPlayer.refHeldObject:GetName())
        -- else
        -- print("Player is hodling obj with name : NONE")
        -- end
        -- print("____ TRY  GRABB _____")

        if self.refHeldObject == nil then
            print("No ref to held obj")
            print("refHeldObject : " .. tostring(self.refHeldObject))
            CheckInteraction(_, _, 15)
        else
            oCurrentEntity = self.refHeldObject
        end
        
        if not oCurrentEntity then
            print("No grabbable in sight")
            return
        end
        
        -- DEBUG
        -- print("oCurrentEntity après raycast : " .. oCurrentEntity:GetName())
        -- print("oCompContainer lookup...")
        oCompContainer = oCurrentEntity:FindChild("components")
        -- print("oCompContainer : " .. oCompContainer:GetName())
        oGGunTargetComp = oCompContainer:FindChild("GGunTargetComponent")

        if not oGGunTargetComp then return end
        if oGGunTargetComp:CanInteract() then
            if self.bIsHoldingObject == 0 then
                oGGunTargetComp:GravityGunGrabb()
            else
                oGGunTargetComp:GravityGunThrow(iThrowForce * iCount / 2)
            end
        end

        -- DEBUG
        -- print("Player name : " .. oPlayer:GetName())
        -- print("Player World pos : {" ..
        -- oPlayer:GetWorldPosition().x .. ", " .. oPlayer:GetWorldPosition().y .. ", " .. oPlayer:GetWorldPosition().z ..
        -- "}")
        -- print("Player Local pos : {" ..
        -- oPlayer:GetPosition().x .. ", " .. oPlayer:GetPosition().y .. ", " .. oPlayer:GetPosition().z .. "}")
        -- print("Player pos TEST : {" .. oPlayer.Pos.x .. ", " .. oPlayer.Pos.y .. ", " .. oPlayer.Pos.z .. "}")
        -- print("*******  ENDING Interaction  *******")
        -- print("")
    end
end

--Set up the component
function self:Setup(pNewEmitter, iNewMaxDistance)
    oPlayer = pNewEmitter
    iMaxDistance = iNewMaxDistance
    pRootNode = self:GetParent()
    while pRootNode:GetName() ~= "SceneRoot" do
        pRootNode = pRootNode:GetParent()
    end
end

function OnInit() end

function OnUpdate(dt)
    iCurDT = dt
end
function OnDestroy() end