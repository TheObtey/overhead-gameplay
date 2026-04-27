---@meta

---@class tween
tween = {}

---@param sTweenId String  Identifier of the tween to create. Must be unique
---@param tFrom Table Starting numeric array
---@param tTo Table Ending numeric array
---@param iDuration Number The tween duration in seconds
---@param fcEase Function Easing function applied to the tween progress. Use nil for linear interpolation
---@param fcUpdateCallback Function Function called every tick with the interpolated value
---@param fcFinishCallback Function Function called at the end of the interpolation
function tween.Create(sTweenId, tFrom, tTo, iDuration, fcEase, fcCallback) end

---@param tFrom Table Starting numeric array
---@param tTo Table Ending numeric array
---@param iDuration Number The tween duration in seconds
---@param fcEase Function Easing function applied to the tween progress. Use nil for linear interpolation
---@param fcUpdateCallback Function Function called every tick with the interpolated value
---@param fcFinishCallback Function Function called at the end of the interpolation
---@return sTweenId String ID of the created tween
function tween.Simple(tFrom, tTo, iDuration, fcEase, fcCallback) end

---@param sTweenId String Identifier of the tween to remove
function tween.Remove(sTweenId) end

---@param sTweenId String Identifier of the tween to check
function tween.Exists(sTweenId) end

--- Clears all the tweens currently running
function tween.ClearAll() end

---@param sTweenId String Identifier of the tween
function tween.Pause(sTweenId) end

---@param sTweenId String Identifier of the tween
function tween.UnPause(sTweenId) end

---@param sTweenId String Identifier of the tween to toggle
function tween.Toggle(sTweenId) end

---@param sTweenId String Identifier of the tween
---@return iProgress Number Tween progress between 0 and 1
function tween.Progress(sTweenId) end

---@param sTweenId String Identifier of the tween
---@return iTimeLeft Number Time left before completion
function tween.TimeLeft(sTweenId) end

---@param iValue Number Value to round
---@param iDecimals Number Number of decimals to display (default 0)
---@return iValue Number Rounded number
function tween.Round(iValue, iDecimals) end