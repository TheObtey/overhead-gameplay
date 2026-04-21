local pEmitter
local iMaxDistance
local oCurrentEntity
local oCompContainer
local oIrcomp
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
    vecForward = pEmitter:GetLocalForward()
    vecForward.z = vecForward.z * -1
-- print("Vec forward : {"..vecForward.x..vecForward.y..vecForward.z.."}")
    local oHit = physics.Raycast(origin or pEmitter:GetPosition(), forward or vecForward, distance or iMaxDistance)

    if not oHit then
        SetCurrentEntity(nil)
        return
    end

    if not oHit.node then
        SetCurrentEntity(nil)
        return
    end

    oCompContainer = oHit.node:FindChild("components")
    if not oCompContainer then return end
    oIrcomp = oCompContainer:FindChild("InteractReceiverComponent")

    if not oIrcomp then
        SetCurrentEntity(oHit.node)
        return
    end

    if oIrcomp:CanInteract() then
        if not oHit or not oHit.GetPrompt then
            print(oIrcomp:GetPrompt())
        else
            print(oHit:GetPrompt())
        end
    end

    SetCurrentEntity(oHit.node)
end

-- Launch the interaction
-- function self:TryInteract()
self.TryInteract = function(icInteract)
    if not oCurrentEntity then return end
    if icInteract:IsPressed() then
        oCompContainer = oCurrentEntity:FindChild("components")
        oIrcomp = oCompContainer:FindChild("InteractReceiverComponent")

        if not oIrcomp then return end

        if oIrcomp:CanInteract() then
            oIrcomp:Interact()
        end

        oCurrentEntity:ResetForces()
    end
end

self.TryGrabb = function(icInteract)
    if icInteract:IsPressed() then
        print("____ TRY  GRABB _____")
        CheckInteraction(_, _, 15)
        
        if not oCurrentEntity then print("No grabbable in sight") return end
        
        oCompContainer = oCurrentEntity:FindChild("components")
        oIrcomp = oCompContainer:FindChild("InteractReceiverComponent")

        if not oIrcomp then return end

        if oIrcomp:CanInteract() then
            oIrcomp:Interact()
        end

        oCurrentEntity:ResetForces()
    end
end
--Set up the component
function self:Setup(pNewEmitter, iNewMaxDistance)
    pEmitter = pNewEmitter
    iMaxDistance = iNewMaxDistance
    
    timer.Create("RaycastDelay", 1, 0, CheckInteraction)
end

function OnInit() end

function OnUpdate(dt) end

function OnDestroy() end
