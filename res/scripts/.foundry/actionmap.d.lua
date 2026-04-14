---@meta

--------------------------------------------------------------------------------------------
-- ButtonState
---@enum ButtonState
ButtonState = {
    UP = false,
    DOWN = true
}

--------------------------------------------------------------------------------------------
-- ControlType
---@enum ControlType
ControlType = {
    BUTTON = 1,
    STICK = 3,

    UNDEFINED = 0
}

--------------------------------------------------------------------------------------------
-- IControl
---@class icontrol
icontrol = {}

---@return controltype
function icontrol:GetControlType() end

---@return eventinput
function icontrol:GetEventInput() end

---@param icIControl icontrol
---@return boolean
function icontrol:ReadAsBool(icIControl) end

---@param icIControl icontrol
---@return number
function icontrol:ReadAsFloat(icIControl) end

---@param icIControl icontrol
---@return vec2
function icontrol:ReadAsVec2(icIControl) end

--------------------------------------------------------------------------------------------
-- Action
---@class action
---@field Event function
action = {}
action.Event = {}

---@param ctControlType controltype Type of control (ex BUTTON)
---@param eiEventInput eventinput Input ID (ex KEY_SPACE == 32)
---@return number the index of the added control
function action:AddControl(ctControlType, eiEventInput) end

---@param iIndex number
---@return icontrol
function action:GetControl(iIndex) end

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

---@param sName string ActionMap name
---@return actionmap
function actionmap:new(sName) end

---@param sName string Action name
---@param ctControlType ControlType
---@param eiEventInput EventInput
---@return action returns instanciated Action
function actionmap:CreateAction(sName, ctControlType, eiEventInput) end

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

---@param sName string action name
---@param ctControlType ControlType controltype
---@param eiEventInput EventInput input ID
function actionmap:CreateAction(sName, ctControlType, eiEventInput) end
