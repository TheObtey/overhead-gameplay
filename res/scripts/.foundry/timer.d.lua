---@meta

---@class timer
timer = {}

---@param sTimerId Identifier of the timer to create. Must be unique
---@param iDelay The delay interval in seconds
---@param iReps The number of times to repeat the timer
---@param fcCallback Function called when timer has finished the countdown
function timer.Create(sTimerId, iDelay, iReps, fcCallback) end

---@param iDelay The delay interval in seconds
---@param fcCallback Function called when timer has finished the countdown
function timer.Simple(iDelay, fcCallback) end

---@param sTimerId Identifier of the timer to remove
function timer.Remove(sTimerId) end

---@param sTimerId Identifier of the timer to check
function timer.Exists(sTimerId) end

--- Clears all the timers currently running
function timer.ClearAll() end

---@param sTimerId Identifier of the timer
function timer.Pause(sTimerId) end

---@param sTimerId Identifier of the timer
function timer.UnPause(sTimerId) end

---@param sTimerId Identifier of the timer to toggle
function timer.Toggle(sTimerId) end

---@param sTimerId Identifier of the timer
function timer.TimeLeft(sTimerId) end

---@param sTimerId Identifier of the timer
function timer.RepsLeft(sTimerId) end

---@param sTimerId Identifier of the timer to adjust
---@param iDelay The delay interval in seconds. Must be specified
---@param iReps The number of times to repeat the timer
---@param fcCallback The new function. Use nil to keep previous value
function timer.Adjust(sTimerId, iDelay, iReps, fcCallback) end