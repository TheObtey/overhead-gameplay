---@type NodeRigidBody

--#region ease
ease = {}
ease.In = {}
ease.Out = {}
ease.InOut = {}

-- Linear
function ease.Linear(iT)
    return iT
end

-- Sine
function ease.In.Sine(iT)
    return 1 - fmath.Cos((iT * fmath.Pi) / 2)
end

function ease.Out.Sine(iT)
    return fmath.Sin((iT * fmath.Pi) / 2)
end

function ease.InOut.Sine(iT)
    return -(fmath.cos(fmath.Pi * iT) - 1) / 2
end

-- Quad
function ease.In.Quad(iT)
    return iT * iT
end

function ease.Out.Quad(iT)
    return 1 - (1 - iT) * (1 - iT)
end

function ease.InOut.Quad(iT)

    if iT < 0.5 then
        return 2 * iT * iT
    end

    return 1 - ((-2 * iT + 2) ^ 2) / 2

end

-- Cubic
function ease.In.Cubic(iT)
    return iT * iT * iT
end

function ease.Out.Cubic(iT)
    return 1 - ((1 - iT) ^ 3)
end

function ease.InOut.Cubic(iT)

    if iT < 0.5 then
        return 4 * iT * iT * iT
    end

    return 1 - ((-2 * iT + 2) ^ 3) / 2

end

-- Quart
function ease.In.Quart(iT)
    return iT ^ 4
end

function ease.Out.Quart(iT)
    return 1 - (1 - iT) ^ 4
end

function ease.InOut.Quart(iT)

    if iT < 0.5 then
        return 8 * iT ^ 4
    end

    return 1 - ((-2 * iT + 2) ^ 4) / 2

end

-- Quint
function ease.In.Quint(iT)
    return iT ^ 5
end

function ease.Out.Quint(iT)
    return 1 - (1 - iT) ^ 5
end

function ease.InOut.Quint(iT)

    if iT < 0.5 then
        return 16 * iT ^ 5
    end

    return 1 - ((-2 * iT + 2) ^ 5) / 2

end

-- Expo
function ease.In.Expo(iT)
    if iT == 0 then return 0 end
    return 2 ^ (10 * iT - 10)
end

function ease.Out.Expo(iT)
    if iT == 1 then return 1 end
    return 1 - 2 ^ (-10 * iT)
end

function ease.InOut.Expo(iT)

    if iT == 0 then return 0 end
    if iT == 1 then return 1 end

    if iT < 0.5 then
        return (2 ^ (20 * iT - 10)) / 2
    end

    return (2 - 2 ^ (-20 * iT + 10)) / 2

end

-- Circ
function ease.In.Circ(iT)
    return 1 - fmath.sqrt(1 - iT * iT)
end

function ease.Out.Circ(iT)
    return fmath.sqrt(1 - (iT - 1) ^ 2)
end

function ease.InOut.Circ(iT)

    if iT < 0.5 then
        return (1 - fmath.sqrt(1 - (2 * iT) ^ 2)) / 2
    end

    return (fmath.sqrt(1 - (-2 * iT + 2) ^ 2) + 1) / 2

end

-- Back
local iC1 = 1.70158
local iC2 = iC1 * 1.525

function ease.In.Back(iT)
    return (iC1 + 1) * iT * iT * iT - iC1 * iT * iT
end

function ease.Out.Back(iT)
    return 1 + (iC1 + 1) * (iT - 1) ^ 3 + iC1 * (iT - 1) ^ 2
end

function ease.InOut.Back(iT)

    if iT < 0.5 then
        return ((2 * iT) ^ 2 * ((iC2 + 1) * 2 * iT - iC2)) / 2
    end

    return ((2 * iT - 2) ^ 2 * ((iC2 + 1) * (iT * 2 - 2) + iC2) + 2) / 2

end

-- Bounce
local function _bounceOut(iT)

    if iT < 1 / 2.75 then
        return 7.5625 * iT * iT
    elseif iT < 2 / 2.75 then
        iT = iT - (1.5 / 2.75)
        return 7.5625 * iT * iT + 0.75
    elseif iT < 2.5 / 2.75 then
        iT = iT - (2.25 / 2.75)
        return 7.5625 * iT * iT + 0.9375
    end

    iT = iT - (2.625 / 2.75)
    return 7.5625 * iT * iT + 0.984375

end

function ease.Out.Bounce(iT)
    return _bounceOut(iT)
end

function ease.In.Bounce(iT)
    return 1 - _bounceOut(1 - iT)
end

function ease.InOut.Bounce(iT)

    if iT < 0.5 then
        return (1 - _bounceOut(1 - 2 * iT)) / 2
    end

    return (1 + _bounceOut(2 * iT - 1)) / 2

end

-- Elastic
local iC4 = (2 * fmath.Pi) / 3
local iC5 = (2 * fmath.Pi) / 4.5

function ease.In.Elastic(iT)

    if iT == 0 then return 0 end
    if iT == 1 then return 1 end

    return -2 ^ (10 * iT - 10) * fmath.sin((iT * 10 - 10.75) * iC4)

end

function ease.Out.Elastic(iT)

    if iT == 0 then return 0 end
    if iT == 1 then return 1 end

    return 2 ^ (-10 * iT) * fmath.sin((iT * 10 - 0.75) * iC4) + 1

end

function ease.InOut.Elastic(iT)

    if iT == 0 then return 0 end
    if iT == 1 then return 1 end

    if iT < 0.5 then
        return -(2 ^ (20 * iT - 10) *
            fmath.sin((20 * iT - 11.125) * iC5)) / 2
    end

    return (2 ^ (-20 * iT + 10) *
        fmath.sin((20 * iT - 11.125) * iC5)) / 2 + 1

end

--#endregion
--#region tween
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

    return math.min(tTween.iElapsed / tTween.iDuration, 1)
end

-- Gets time left before completion
function tween.TimeLeft(sTweenId)
    assert(type(sTweenId) == "string", "tween.TimeLeft: id must be a string")

    local tTween = _getTween(sTweenId)
    if not tTween then return end

    return math.max(0, tTween.iDuration - tTween.iElapsed)
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
--#endregion

local self = self

local oReceiver
local oTarget

--Check if the door has to go to the origin or the target position
local bIsOpen = false

-- origin and target positions
local tOriginPos
local tTargetPos

--Callback to set the interaction to true when the tween has finish
local function ResetInteract()
    oReceiver:SetInteract(true)
end

local function UpdateTranslation(tValue)
    print(#tValue)
    print(tTargetPos.x, tTargetPos.y)

    --local vecNewPos = fmath.vec3:new(tValue[1], tValue[2], tValue[3])
end

--Interaction behaviour
function self:Interaction()

    oReceiver:SetInteract(false)
    if bIsOpen == true then
        bIsOpen = false
        tween.Create("Tween:Door:translation", tTargetPos, tOriginPos, 2, nil, UpdateTranslation, ResetInteract)
    else
        bIsOpen = true
        tween.Create("Tween:Door:translation", tOriginPos, tTargetPos, 2, nil , UpdateTranslation, ResetInteract)
    end
end
--ease.In.Back
function OnInit()

    oTarget = self:FindChild("N3D_Target"):As(NodeTypes.NODE3D)

    local compContainer = self:FindChild("components")
    oReceiver = compContainer:FindChild("InteractReceiverComponent")

    local vecOriginPos = self:GetPosition()
    tOriginPos =
    {
        x = vecOriginPos.x,
        y = vecOriginPos.y,
        z = vecOriginPos.z
    }

    local vecTargetPos = oTarget:GetWorldPosition()
    tTargetPos = 
    {
        x = vecTargetPos.x,
        y = vecTargetPos.y,
        z = vecTargetPos.z
    }
end

function OnUpdate(iDelta)
    tween.Tick(iDelta)
end

function OnDestroy()
end
