---@type NodeRigidBody

local self = self
local oActivatorComponent
local oReceiverComponent
local bIsActivated = false

local function ResetInteraction()
    oReceiverComponent:SetInteract(true)
end

function self:Interaction()
    oReceiverComponent:SetInteract(false) --Stop the capacity to interact
    timer.Simple(2, ResetInteraction)
    bIsActivated = not bIsActivated

    oActivatorComponent:DoAction()
end

function self:GetPrompt()
    if bIsActivated == false then
        return "Press F to open the door"
    else
        return "Press F to close the door"
    end
end

function OnInit()
    local oCompContainer = self:FindChild("components")
    oReceiverComponent = oCompContainer:FindChild("InteractReceiverComponent")
    oActivatorComponent = self:GetNode("/SceneRoot/RB_Door/components/ActivableComponent")

    if not oActivatorComponent then return end
end

function OnUpdate(iDelta) end

function OnDestroy() end