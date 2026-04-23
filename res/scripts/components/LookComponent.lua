---@class node
self = self

local oRB
local oWindow
local oCameraRoot

local csCurrentCursorState = CursorState.LOCKED
local iMouseSensitivity = 0.2
local iMinPitch = -fmath.Pi 
local iMaxPitch = fmath.Pi 
local iCurPitch = 0.0

self.HandleMouseLook = function(icMouse)
    local vecMouse = icontrol.ReadAsVec2(icMouse)

    if oRB.bIsRotating then return end

    if not oRB or not oCameraRoot or csCurrentCursorState ~= CursorState.LOCKED then return end
local v = (vecMouse.y + 0.017905) * 5
    local iMouseInput = v * iMouseSensitivity
    local iNewPitch = fmath.Clamp(iCurPitch + iMouseInput, iMinPitch, iMaxPitch)
    local iDelta =  iNewPitch - iCurPitch
    iCurPitch = iCurPitch + iDelta
    oCameraRoot:AddLocalPitch(iDelta)
    -- oCameraRoot:AddLocalYaw((-vecMouse.x * 3) * iMouseSensitivity * oRB.gravity)

    oRB:ApplyWorldTorque(fmath.vec3:new(0, (-vecMouse.x * 3) * iMouseSensitivity * 20000 * oRB.gravity, 0))
end

local iLastChange = 0
self.OnCursorStateChangePress = function(icContext)
    local iCurTime = os.clock()
    if iLastChange > iCurTime then return end
    iLastChange = iCurTime + .2

    self:SetCursorState(csCurrentCursorState == CursorState.FREE and CursorState.LOCKED or CursorState.FREE)
end

function self:SetCursorState(csNewState)
    assert(csNewState == CursorState.FREE or csNewState == CursorState.LOCKED or  csNewState == CursorState.HIDDEN, "LookComponent: Valid cursor state expected")

    csCurrentCursorState = csNewState

    if oWindow and oWindow:Is(NodeTypes.NODE_WINDOW) then
        oWindow:SetCursorState(csNewState)
    end
end

function self:Setup(oNewRigidBody, oNewWindow, oNewCameraRoot)
    oRB = oNewRigidBody
    oWindow = oNewWindow:As(NodeTypes.NODE_WINDOW)
    oCameraRoot = oNewCameraRoot:As(NodeTypes.NODE3D)

    if oWindow and oWindow:Is(NodeTypes.NODE_WINDOW) then
        oWindow:SetCursorState(csCurrentCursorState)
    end

    print("LookComponent Initialized")
end

function OnInit()
end

function OnUpdate(iDelta)
end