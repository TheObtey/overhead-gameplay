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
    local hit = physics.Raycast(pEmitter:GetPosition(), vecForward, iMaxDistance)

    if not hit then
        SetCurrentEntity(nil)
        return
    end

    if not hit.node then
        SetCurrentEntity(nil)
        return
    end

    oCompContainer = hit.node:FindChild("components")
    oIrcomp = oCompContainer:FindChild("InteractReceiverComponent")

    if not oIrcomp then
        SetCurrentEntity(hit.node)
        return
    end

    if oIrcomp:CanInteract() then
        print(oIrcomp:GetPrompt())
    end

    SetCurrentEntity(hit.node)
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
    timer.Create("RaycastDelay", 1.5, 1000000, CheckInteraction)
end

function OnInit()  end

function OnUpdate(dt) end

function OnDestroy() end