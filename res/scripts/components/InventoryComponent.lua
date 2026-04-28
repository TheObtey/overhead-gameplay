---@class node
local self = self

local tItems = {}

-- Constructor
---@return inventory
function self:new()
    tItems = {}
    return self
end

-- Append Item in Inventory
function self:AddItem(sName, iCount)
    iCount = iCount or 1
    
    if (tItems[sName] == nil) then
        tItems[sName] = iCount
    end

    tItems[sName] = tItems[sName] + 1
end

-- Erase Item from Inventory
function EraseItem(sName)
    if (tItems[sName] ~= nil) then
        tItems[sName] = nil
    end
end

-- Simply returns if Player possesses at least one copy of the item
function self:HasItem(sName)
    if (tItems[sName] ~= nil and tItems[sName] > 0) then
        return true
    else
        return false
    end
end
