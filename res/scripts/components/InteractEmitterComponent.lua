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
local function CheckInteraction()
    --pEmitter:GetLocalForward()*-1
    local oHit = physics.Raycast(pEmitter:GetPosition(), pEmitter:GetLocalForward()*-1, iMaxDistance)

    if not oHit then
        SetCurrentEntity(nil)
        return
    end

    if not oHit.node then
        SetCurrentEntity(nil)
        return
    end

    oCompContainer = oHit.node:FindChild("InteractReceiverComponent")
    if oCompContainer then print("oeoeoe") end
--components
    oIrcomp = oCompContainer:FindChild("InteractReceiverComponent")

    if not oIrcomp then
        SetCurrentEntity(oHit.node)
        return
    end

    if oIrcomp:CanInteract() then
        print(oIrcomp:GetPrompt())
    end

    SetCurrentEntity(oHit.node)
end

-- Launch the interaction
function self:TryInteract()
    if not oCurrentEntity then return end

    oCompContainer = oCurrentEntity:FindChild("components")
    oIrcomp = oCompContainer:FindChild("InteractReceiverComponent")

    if not oIrcomp then return end

    if oIrcomp:CanInteract() then
        oIrcomp:Interact()
    end
end

--Set up the component
function self:Setup(pNewEmitter, iNewMaxDistance)
    pEmitter = pNewEmitter
    iMaxDistance = iNewMaxDistance
    vecForward = fmath.vec3:new(0, 0, -1)
    timer.Create("RaycastDelay", 3, 0, CheckInteraction)
end

function OnInit()  end

function OnUpdate(dt) end

function OnDestroy() end