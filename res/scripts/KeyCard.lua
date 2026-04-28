---@type NodeRigidBody

local self = self
local oCompContainer
local oReceiverComponent

local bIsCollected = false

local function ResetInteraction()
    oReceiverComponent:SetInteract(true)
end

self.Interaction = function(pEmitter)
    assert(pEmitter ~= nil, "L'emitter est nil")

    if bIsCollected == true then
        return 
    end

    local oInventoryComponent = pEmitter:GetNode("components/InventoryComponent")
    oInventoryComponent:AddItem("KeyCard")
    bIsCollected = true
    oReceiverComponent:SetInteract(false)
    --self:Destroy() 
end

function self:GetPrompt()
    return "Press F to collect the key"
end

function OnInit()
    oCompContainer = self:FindChild("components")
    oReceiverComponent = oCompContainer:FindChild("InteractReceiverComponent")
end

function OnUpdate(iDelta) end

function OnDestroy() end