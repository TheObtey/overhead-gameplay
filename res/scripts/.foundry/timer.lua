timer = {}
timer._timers = {}

local iNow = 0.0
local iUid = 0

local function _tostring(x)
    local bOk, sVal = pcall(tostring, x)
    return bOk and sVal or "<tostring error>"
end

local function _traceback(err)
    return debug.traceback(_tostring(err), 2)
end

local function _logError(sTimerId, err)
    local sMsg = "[timer] error in timer '".._tostring(sTimerId).."':\n"    
    sMsg = sMsg.._traceback(err)

    if type(LogError) == "function" then
        LogError(sMsg)
    elseif type(Log) == "function" then
        Log(sMsg)
    else
        print(sMsg)
    end
end

-- Returns a unique id for simple timers.
local function _newId()
    iUid = iUid + 1
    return "__simple_"..iUid
end

-- Returns a timer table by name
local function _getTimer(sTimerId)
    return timer._timers[sTimerId]
end

-- Schedules the next fire time for a timer
local function _scheduleNext(tTimer)
    tTimer.iNext = iNow + tTimer.iDelay
end

-- Returns current internal clock
function timer.Now()
    return iNow
end

-- Creates a named timer
function timer.Create(sTimerId, iDelay, iReps, fcCallback)
    assert(type(sTimerId) == "string", "timer.Create: id must be a string")
    assert(type(iDelay) == "number" and iDelay >= 0, "timer.Create: delay must be a positive number")
    assert(type(iReps) == "number" and iReps >= 0, "timer.Create: repetition must be a positive number")
    assert(type(fcCallback) == "function", "timer.Create: valid function expected")
    
    local tTimer = {
        sId = sTimerId,
        iDelay = iDelay,
        iReps = iReps,
        fcCallback = fcCallback,

        iRan = 0,
        bPaused = false,
        iNext = iNow + iDelay,
        iRemaining = nil
    }

    timer._timers[sTimerId] = tTimer
end

-- Creates a one-shot anonymous timer and return its id
function timer.Simple(iDelay, fcCallback)
    assert(type(iDelay) == "number" and iDelay >= 0, "timer.Simple: delay must be a positive number")
    assert(type(fcCallback) == "function", "timer.Simple: valid function expected")
    
    local sTimerId = _newId()
    timer.Create(sTimerId, iDelay, 1, fcCallback)

    return sTimerId
end

-- Removes a timer by id
function timer.Remove(sTimerId)
    assert(type(sTimerId) == "string", "timer.Remove: id must be a string")
    timer._timers[sTimerId] = nil
end

-- Checks if a timer exists
function timer.Exists(sTimerId)
    assert(type(sTimerId) == "string", "timer.Exists: id must be a string")
    return timer._timers[sTimerId] ~= nil
end

-- Clears all timers
function timer.ClearAll()
    timer._timers = {}
end

-- Pauses a timer
function timer.Pause(sTimerId)
    assert(type(sTimerId) == "string", "timer.Pause: id must be a string")

    local tTimer = _getTimer(sTimerId)
    if not tTimer then return end
    if tTimer.bPaused then return end

    tTimer.bPaused = true
    tTimer.iRemaining = math.max(0, (tTimer.iNext or iNow) - iNow)
end

-- UnPauses a timer
function timer.UnPause(sTimerId)
    assert(type(sTimerId) == "string", "timer.UnPause: id must be a string")

    local tTimer = _getTimer(sTimerId)
    if not tTimer then return end
    if not tTimer.bPaused then return end

    tTimer.bPaused = false

    local iRem = tTimer.iRemaining
    tTimer.iRemaining = nil

    if iRem == nil then
        _scheduleNext(tTimer)
        return
    end

    tTimer.iNext = iNow + iRem
end

-- Toggles a timer pause state
function timer.Toggle(sTimerId)
    assert(type(sTimerId) == "string", "timer.Toggle: id must be a string")

    local tTimer = _getTimer(sTimerId)
    if not tTimer then return end

    if tTimer.bPaused then
        timer.UnPause(sTimerId)
        return
    end

    timer.Pause(sTimerId)
end

-- Gets time left before next execution
function timer.TimeLeft(sTimerId)
    assert(type(sTimerId) == "string", "timer.TimeLeft: id must be a string")

    local tTimer = _getTimer(sTimerId)
    if not tTimer then return end

    if tTimer.bPaused then
        return tTimer.iRemaining or 0
    end

    return math.max(0, (tTimer.iNext or iNow) - iNow)
end

-- Gets repetitions left
function timer.RepsLeft(sTimerId)
    assert(type(sTimerId) == "string", "timer.RepsLeft: id must be a string")

    local tTimer = _getTimer(sTimerId)
    if not tTimer then return end

    if tTimer.iReps == 0 then return 0 end

    return math.max(0, tTimer.iReps - tTimer.iRan)
end

-- Adjusts an existing timer
function timer.Adjust(sTimerId, iDelay, iReps, fcCallback)
    assert(type(sTimerId) == "string", "timer.Adjust: id must be a string")

    local tTimer = _getTimer(sTimerId)
    
    assert(type(tTimer) ~= nil, "timer.Adjust: timer does not exist")
    assert(type(iDelay) == "number" and iDelay >= 0, "timer.Adjust: delay must be a positive number")

    if iReps ~= nil then
        assert(type(iReps) == "number" and iReps >= 0, "timer.Adjust: repetition must be a positive number")
        tTimer.iReps = iReps
    end

    if fcCallback ~= nil then
        assert(type(fcCallback) == "function", "timer.Adjust: valid function expected")
        tTimer.fcCallback = fcCallback
    end

    tTimer.iDelay = iDelay

    if tTimer.bPaused then
        tTimer.iRemaining = iDelay
        return
    end

    _scheduleNext(tTimer)
end

-- Ticks timers using delta time
function timer.Tick(iDt, iMaxFiresPerTick)
    assert(type(iDt) == "number" and iDt >= 0, "timer.Tick: delta time must be a positive number")

    iNow = iNow + iDt
    iMaxFiresPerTick = iMaxFiresPerTick or 1000
    
    for sId, tTimer in pairs(timer._timers) do
        if not tTimer.bPaused then
            
            local iFires = 0

            while tTimer.iNext and iNow >= tTimer.iNext do
                iFires = iFires + 1
                
                if iFires > iMaxFiresPerTick then
                    _logError(sId, "max fires per tick reached")
                    break
                end

                local bOk, err = pcall(tTimer.fcCallback)
                if not bOk then
                    _logError(sId, err)
                end

                tTimer.iRan = tTimer.iRan + 1

                if tTimer.iReps ~= 0 and tTimer.iRan >= tTimer.iReps then
                    timer._timers[sId] = nil
                    break
                end

                tTimer.iNext = tTimer.iNext + tTimer.iDelay
            end
        end
    end
end