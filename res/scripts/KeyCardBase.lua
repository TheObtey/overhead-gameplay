---@type Node
self = self

local bIsPickedUp

-- Recuperation de la KeyCard
function self:Interaction(pEmitter)
    if pEmitter == nil then
        return
    end

    local oInventoryComponent = pEmitter:GetNode("components/InventoryComponent")

    if oInventoryComponent == nil then
        return
    end

    if bIsPickedUp then
        return
    end

    oInventoryComponent:AddItem("KeyCard")
    bIsPickedUp = true
end


function OnInit()
    bIsPickedUp = false
end

function OnUpdate(dt)
end

function OnDestroy()
end
