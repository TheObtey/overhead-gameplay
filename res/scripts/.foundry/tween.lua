tween = {}
tween._tweens = {}

local iUid = 0

local function _newId()
    iUid = iUid + 1
    return "__tween_"..iUid
end

local function _getTween(sTweenId)
    return tween._tweens[sTweenId]
end

-- Linear interpolate between two numbers
local function _lerp(iFrom, iTo, iProgress)
    return iFrom + (iTo - iFrom) * iProgress
end

-- Copies a numeric array
local function _deepCopy(tValue)
    local tCopy = {}

    for iIndex = 1, #tValue do
        tCopy[iIndex] = tValue[iIndex]
    end

    return tCopy
end

-- Interpolates between two numeric arrays
local function _lerpTable(tFrom, tTo, iProgress)
    local tValue = {}

    for iIndex = 1, #tFrom do
        tValue[iIndex] = _lerp(tFrom[iIndex], tTo[iIndex], iProgress)
    end

    return tValue
end

-- Applies easing to a progress value
local function _ease(fcEase, iProgress)
    if fcEase == nil then return iProgress end
    return fcEase(iProgress)
end

-- Creates a named tween
function tween.Create(sTweenId, tFrom, tTo, iDuration, fcEase, fcUpdateCallback, fcFinishCallback)
    assert(type(sTweenId) == "string", "tween.Create: id must be a string")
    assert(type(tFrom) == "table", "tween.Create: from must be a table")
    assert(type(tTo) == "table", "tween.Create: to must be a table")
    assert(#tFrom == #tTo, "tween.Create: from and to must have the same size")
    assert(type(iDuration) == "number" and iDuration >= 0, "tween.Create: duration must be a positive number")
    assert(type(fcUpdateCallback) == "function", "tween.Create: valid update function expected")
    assert(type(fcFinishCallback) == "function", "tween.Create: valid finish function expected")

    tween._tweens[sTweenId] = {
        sId = sTweenId,
        tFrom = _deepCopy(tFrom),
        tTo = _deepCopy(tTo),
        iDuration = iDuration,
        iElapsed = 0,
        fcEase = fcEase,
        fcUpdateCallback = fcUpdateCallback,
        fcFinishCallback = fcFinishCallback,
        bPaused = false
    }

    if iDuration == 0 then
        fcUpdateCallback(_deepCopy(tTo))
        fcFinishCallback()
        tween._tweens[sTweenId] = nil
    end
end

-- Craetes an anonymous tween and returns its id
function tween.Simple(tFrom, tTo, iDuration, fcEase, fcUpdateCallback, fcFinishCallback)
    local sTweenId = _newId()
    tween.Create(sTweenId, tFrom, tTo, iDuration, fcEase, fcUpdateCallback, fcFinishCallback)

    return sTweenId
end

-- Removes a tween by id
function tween.Remove(sTweenId)
    assert(type(sTweenId) == "string", "tween.Remove: id must be a string")
    tween._tweens[sTweenId] = nil
end

-- Checks if a tween exists
function tween.Exists(sTweenId)
    assert(type(sTweenId) == "string", "tween.Exists: id must be a string")
    return tween._tweens[sTweenId] ~= nil
end

-- Clears all tweens
function tween.ClearAll()
    tween._tweens = {}
end

-- Pauses a tween
function tween.Pause(sTweenId)
    assert(type(sTweenId) == "string", "tween.Pause: id must be a string")

    local tTween = _getTween(sTweenId)
    if not tTween then return end

    tTween.bPaused = true
end

-- Unpauses a tween
function tween.UnPause(sTweenId)
    assert(type(sTweenId) == "string", "tween.UnPause: id must be a string")

    local tTween = _getTween(sTweenId)
    if not tTween then return end

    tTween.bPaused = false
end

-- Toggles a tween pause state
function tween.Toggle(sTweenId)
    assert(type(sTweenId) == "string", "tween.Toggle: id must be a string")

    local tTween = _getTween(sTweenId)
    if not tTween then return end

    tTween.bPaused = not tTween.bPaused
end

-- Gets tween progress between 0 and 1
function tween.Progress(sTweenId)
    assert(type(sTweenId) == "string", "tween.Progress: id must be a string")

    local tTween = _getTween(sTweenId)
    if not tTween then return end

    if tTween.iDuration == 0 then return 1 end

    return fmath.Min(tTween.iElapsed / tTween.iDuration, 1)
end

-- Gets time left before completion
function tween.TimeLeft(sTweenId)
    assert(type(sTweenId) == "string", "tween.TimeLeft: id must be a string")

    local tTween = _getTween(sTweenId)
    if not tTween then return end

    return fmath.Max(0, tTween.iDuration - tTween.iElapsed)
end

-- Returns the given value rounded with the specified amount of decimals
function tween.Round(iValue, iDecimals)
    iDecimals = iDecimals or 0
    local iMultiplier = 10 ^ iDecimals
    local iHalf = 0.50000000000008

    if iValue > 0 then
        return fmath.Floor(iValue * iMultiplier + iHalf) / iMultiplier
    else
        return fmath.Ceil(iValue * iMultiplier - iHalf) / iMultiplier
    end
end

-- Ticks tweens using delta time
function tween.Tick(iDt)
    assert(type(iDt) == "number" and iDt >= 0, "tween.Tick: dt must be a positive number")

    for sTweenId, tTween in pairs(tween._tweens) do
        if not tTween.bPaused then
            tTween.iElapsed = tTween.iElapsed + iDt

            local iProgress = tTween.iElapsed / tTween.iDuration

            if tTween.iDuration == 0 then
                iProgress = 1
            elseif iProgress > 1 then
                iProgress = 1
            end

            local iEasedProgress = _ease(tTween.fcEase, iProgress)
            local tValue = _lerpTable(
                tTween.tFrom,
                tTween.tTo,
                iEasedProgress
            )

            tTween.fcUpdateCallback(tValue)

            if iProgress >= 1 then
                tTween.fcFinishCallback()
                tween._tweens[sTweenId] = nil
            end
        end
    end
end

return tween