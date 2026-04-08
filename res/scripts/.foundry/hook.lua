hook = {}
hook._events = {}

local function _tostring(x)
    local bOk, sVal = pcall(tostring, x)
    return bOk and sVal or "<tostring error>"
end

local function _traceback(err)
    return debug.traceback(_tostring(err), 2)
end

local function _logError(sEventName, sId, err)
    local sMsg = "[hook] error in event '".._tostring(sEventName).."'"
    if sId ~= nil then
        sMsg = sMsg.." (id=".._tostring(sId)..")"
    end
    
    sMsg = sMsg..":\n".._traceback(err)

    if type(LogError) == "function" then
        LogError(sMsg)
    elseif type(Log) == "function" then
        Log(sMsg)
    else
        print(sMsg)
    end
end

-- Adds a callback to an event
function hook.Add(sEventName, sId, fcCallback)
    assert(type(sEventName) == "string", "hook.Add: event name must be a string")
    assert(sId ~= nil, "hook.Add: id must not be nil")
    assert(type(fcCallback) == "function", "hook.Add: valid function expected")
    
    local bucket = hook._events[sEventName]
    if not bucket then
        bucket = {}
        hook._events[sEventName] = bucket
    end

    bucket[sId] = fcCallback
end

-- Removes a callback from an event
function hook.Remove(sEventName, sId)
    local bucket = hook._events[sEventName]
    if not bucket then return end
    bucket[sId] = nil
end

-- Calls all callbacks for an event
function hook.Call(sEventName, ...)
    local bucket = hook._events[sEventName]
    if not bucket then return nil end

    local callbacks = {}
    local ids = {}
    local n = 0

    for id, fn in pairs(bucket) do
        n = n + 1
        callbacks[n] = fn
        ids[n] = id
    end

    for i = 1, n do
        local bOk, sResult = pcall(callbacks[i], ...)
        if not bOk then
            _logError(sEventName, ids[i], sResult)
        end
    end
end

-- Clears a single event
function hook.Clear(sEventName)
    hook._events[sEventName] = nil
end

-- Clears every events
function hook.ClearAll()
    hook._events = {}
end

-- Removes any hooks with id starting with prefix
function hook.RemoveByIdPrefix(sIdPrefix)
    if sIdPrefix == nil then return end
    sIdPrefix = _tostring(sIdPrefix)

    for eventName, bucket in pairs(hook._events) do
        for id, _ in pairs(bucket) do
            local sId = _tostring(id)
            if string.sub(sId, 1, #sIdPrefix) == sIdPrefix then
                bucket[id] = nil
            end
        end
    end
end