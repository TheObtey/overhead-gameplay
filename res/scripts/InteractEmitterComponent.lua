local parent
local oCurrentEntity
local maxDistance

local function SetCurrentEntity(ndHit)
    if oCurrentEntity == ndHit then
        return
    end

    oCurrentEntity = ndHit
end

local function CheckInteraction()
    local hit = physics.Raycast(parent:GetPosition(), parent:GetLocalForward()*-1, 1)

    if not hit then
        print("there is nothing there, clement")
        SetCurrentEntity(nil)
        return
    end

    if not hit.node then
        print("Node not found in the hit")
        SetCurrentEntity(nil)
        return
    end

    local ircomp = hit.node:FindChild("InteractRecieverComponent")

    if not ircomp then
        print("This node doesn't have interact reciever component")
        SetCurrentEntity(hit.node)
        return
    end

    if ircomp:CanInteract() then
        print(ircomp:GetPrompt())
    end

    SetCurrentEntity(hit.node)
end

local function TryInteract()
    if not oCurrentEntity then return end

    local ircomp = oCurrentEntity:FindChild("InteractRecieverComponent")

    if not ircomp then
        print("Cannot interact with"..oCurrentEntity:GetName())
        return
    end

    if ircomp:CanInteract() then
        ircomp:Interact()
    else
        print("Interactable, but not now because I decided so")
    end
end

local function InitActionMap()

    actionmap:GetAction("INTERACT").Event = TryInteract
end

function OnInit() 
    parent = self:GetNode3DParent()
    InitActionMap()

    timer.Create("RaycastDelay", 3, 100000, CheckInteraction)
end

function OnUpdate(dt)
    
end

function OnDestroy()
    
end
