---@meta

---@class hook
hook = {}

---@param sEventName The name of the event to create
---@param sId ID of the hook to create
---@param fcCallback Callback of the hook to create 
function hook.Add(sEventName, sId, fcCallback) end

---@param sEventName The name of the event to remove
---@param sId ID of the hook to remove
function hook.Remove(sEventName, sId) end

---@param sEventName The name of the event to call
---@param VarArgs Data to send to the event
function hook.Call(sEventName, ...) end

--- Called to clear every events
function hook.ClearAll() end

---@param sIdPrefix ID prefix of the events to remove
function hook.RemoveByIdPrefix(sIdPrefix) end