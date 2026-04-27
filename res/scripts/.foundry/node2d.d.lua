---@meta

---@class node2d : Node
node2d = {}

---@overload fun(scale:fmath.vec2)
---@overload fun(scaleX:number, scaleY:number)
function node2d:SetScale(scaleX, scaleY) end

---@return fmath.vec2
function node2d:GetScale() end

---@param theta number
function node2d:SetRotation(theta) end

---@return number
function node2d:GetRotation() end

---@overload fun(pos:fmath.vec2)
---@overload fun(posX:number, posY:number)
function node2d:SetPosition(posX, posY) end

---@return fmath.vec2
function node2d:GetPosition() end

---@overload fun(shearing:fmath.vec2)
---@overload fun(shearingX:number, shearingY:number)
function node2d:SetShearing(shearingX, shearingY) end

---@return fmath.vec2
function node2d:GetShearing() end

---@param statism boolean
function node2d:SetStatism(statism) end

---@return boolean
function node2d:IsStatic() end

---@overload fun(scale:fmath.vec3)
---@overload fun(sX:number, sY:number, sZ:number)
function node2d:SetWorldScale(sX,sY,sZ) end

---@param rotation number
function node2d:SetWorldRotationAngle(rotation) end

---@overload fun(pos:fmath.vec3)
---@overload fun(posX:number, posY:number, posZ:number)
function node2d:SetWorldPosition(posX,posY,posZ) end