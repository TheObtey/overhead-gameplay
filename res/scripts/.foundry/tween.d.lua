---@meta

---@class tween
tween = {}

---@param sTweenId Identifier of the tween to create. Must be unique
---@param tFrom Starting numeric array
---@param tTo Ending numeric array
---@param iDuration The tween duration in seconds
---@param fcEase Easing function applied to the tween progress. Use nil for linear interpolation
---@param fcCallback Function called every tick with the interpolated value
function tween.Create(sTweenId, tFrom, tTo, iDuration, fcEase, fcCallback) end

---@param tFrom Starting numeric array
---@param tTo Ending numeric array
---@param iDuration The tween duration in seconds
---@param fcEase Easing function applied to the tween progress. Use nil for linear interpolation
---@param fcCallback Function called every tick with the interpolated value
function tween.Simple(tFrom, tTo, iDuration, fcEase, fcCallback) end

---@param sTweenId Identifier of the tween to remove
function tween.Remove(sTweenId) end

---@param sTweenId Identifier of the tween to check
function tween.Exists(sTweenId) end

--- Clears all the tweens currently running
function tween.ClearAll() end

---@param sTweenId Identifier of the tween
function tween.Pause(sTweenId) end

---@param sTweenId Identifier of the tween
function tween.UnPause(sTweenId) end

---@param sTweenId Identifier of the tween to toggle
function tween.Toggle(sTweenId) end

---@param sTweenId Identifier of the tween
function tween.Progress(sTweenId) end

---@param sTweenId Identifier of the tween
function tween.TimeLeft(sTweenId) end