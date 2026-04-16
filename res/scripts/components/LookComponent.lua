---@class node
self = self

local oRB
local oWindow
local oCameraRoot

local csCurrentCursorState = CursorState.LOCKED
local iMouseSensitivity = 0.2
local iMinPitch = -89.0
local iMaxPitch = 89.0

self.HandleMouseLook = function(icMouse)
    local vecMouse = icontrol.ReadAsVec2(icMouse)

    if not oRB or not oCameraRoot or csCurrentCursorState ~= CursorState.LOCKED then return end

    local iCurrentPitch = oCameraRoot:GetLocalPitch()
    local iNewPitch = fmath.Clamp(iCurrentPitch + (vecMouse.y * iMouseSensitivity), iMinPitch, iMaxPitch)
    local iDelta = iNewPitch - iCurrentPitch

    oCameraRoot:AddLocalPitch(iDelta)
    oRB:ApplyWorldTorque(fmath.vec3:new(0, (-vecMouse.x * 3) * iMouseSensitivity * 2000, 0))
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