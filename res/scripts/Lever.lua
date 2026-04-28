---@type NodeRigidBody

local self = self
local oCompContainer
local oReceiverComponent
local oActivatorComponent

local bIsActivated = false

local function ResetInteraction()
    oReceiverComponent:SetInteract(true)
end

local function HasKeyCard()
    local oInventoryComponent = self:GetNode("/SceneRoot/Player/components/InventoryComponent")
    assert(oInventoryComponent ~= nil, "Inventory is nil")

    if oInventoryComponent:HasItem("KeyCard") then
        return true
    else 
        return false
    end
end

function self:Interaction(pEmitter)
    oReceiverComponent:SetInteract(false) --Stop the capacity to interact

    if HasKeyCard() == true then
        timer.Simple(2, ResetInteraction)
        bIsActivated = not bIsActivated
        oActivatorComponent:DoAction()
    else
        ResetInteraction()
    end
end

function self:GetPrompt()
    if bIsActivated == false then
        return "Press F to open the door"
    else
        return "Press F to close the door"
    end
end

function OnInit()
    oCompContainer = self:FindChild("components")
    oReceiverComponent = oCompContainer:FindChild("InteractReceiverComponent")
    oActivatorComponent = self:GetNode("/SceneRoot/RB_Door/components/ActivableComponent")
end

function OnUpdate(iDelta) end

function OnDestroy() end