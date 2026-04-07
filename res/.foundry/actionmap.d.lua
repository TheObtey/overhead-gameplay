---@meta

--------------------------------------------------------------------------------------------
-- ButtonState
---@enum buttonstate
buttonstate = {
    UP = false,
    DOWN = true
}

--------------------------------------------------------------------------------------------
-- ControlType
---@enum controltype
controltype = {
    BUTTON = 1,
    SLIDER = 2,
    STICK = 3,

    UNDEFINED = 0
}

--------------------------------------------------------------------------------------------
-- IControl
---@class icontrol
icontrol = {}

---@param ctType controltype
---@param eiEventInput eventinput
---@return icontrol
function icontrol:new(ctType, eiEventInput) end

---@return controltype
function icontrol:GetControlType() end

---@return eventinput
function icontrol:GetEventInput() end

---@param icIControl icontrol
---@return boolean
function icontrol:Read(icIControl) end

---@param icIControl icontrol
---@return number
function icontrol:Read(icIControl) end

---@param icIControl icontrol
---@return vec2
function icontrol:Read(icIControl) end

---@param acAction action
function icontrol:SetAction(acAction) end

--------------------------------------------------------------------------------------------
-- Event
---@class event
event = {}

--------------------------------------------------------------------------------------------
-- Action
---@class action
---@field Event function
action = {}
action.Event = {}

---@return action
function action:new() end

---@param ctControlType controltype Type of Control to trigger the callback event
---@param eiEventInput eventinput Input ID (ex. KEY_A or GAMEPAD_TRIGGER)
---@return action
function action:new(ctControlType, eiEventInput) end

---@param ctControlType controltype
---@param eiEventInput eventinput
---@return number
function action:AddControl(ctControlType, eiEventInput) end

---@return icontrol
function action:GetControl() end

--------------------------------------------------------------------------------------------
-- ButtonControl
---@class buttoncontrol
buttoncontrol = {}

---@param eiEventInput eventinput
---@param acAction action
---@return buttoncontrol
function buttoncontrol:new(eiEventInput, acAction) end

---@return buttonstate
function buttoncontrol:GetState() end

--------------------------------------------------------------------------------------------
-- SliderControl
---@class slidercontrol
slidercontrol = {}

---@param eiEventInput eventinput
---@param acAction action
---@return slidercontrol
function slidercontrol:new(eiEventInput, acAction) end

---@return number
function slidercontrol:GetPos() end

--------------------------------------------------------------------------------------------
-- StickControl
---@class stickcontrol
stickcontrol = {}

---@param eiEventInput eventinput
---@param acAction action
---@return stickcontrol
function stickcontrol:new(eiEventInput, acAction) end

---@return vec2 
function stickcontrol:GetPos() end

--------------------------------------------------------------------------------------------
-- ActionMap
---@class actionmap
actionmap = {}

---@return actionmap
function actionmap:new() end

---@param sName string
---@param acAction action
---@return boolean
function actionmap:Emplace(sName, acAction) end

---@param sName string
---@return boolean
function actionmap:Erase(sName) end

---@param sName string
---@return boolean
function actionmap:GetAction(sName) end

---@return number
function actionmap:Length() end

---@param sOldName string
---@param sNewName string
function actionmap:Rename(sOldName, sNewName) end 
