---@meta

---@class nodecollider
nodecollider = {}

---@param name string
function CreateNodeCollider(name) end


--- =========== Local transform (offset from RigidBody) ===========

---@overload fun(pos:fmath.vec3)
---@overload fun(posX:number, posY:number, posZ:number)
function nodecollider:SetLocalPosition(posX,posY,posZ) end

---@param rot fmath.vec3
function nodecollider:SetLocalRotation(rot) end

---@return fmath.vec3
function nodecollider:GetLocalPosition() end

---@return fmath.vec3
function nodecollider:GetLocalRotation() end


--- =========== Material ===========

---@param bounciness number [0,1]
function nodecollider:SetBounciness(bounciness) end

---@return number
function nodecollider:GetBounciness() end

---@param friction number
function nodecollider:SetFrictionCoefficient(friction) end

---@return number
function nodecollider:GetFrictionCoefficient() end

---@param density number
function nodecollider:SetMassDensity(density) end

---@return number
function nodecollider:GetMassDensity() end



--- =========== Collider behavior ===========

---@param trigger boolean
function nodecollider:SetIsTrigger(trigger) end

---@return boolean
function nodecollider:IsTrigger() end

---@param enabled boolean
function nodecollider:SetIsSimulationCollider(enabled) end

---@return boolean
function nodecollider:IsSimulationCollider() end

---@param enabled boolean
function nodecollider:SetIsWorldQueryCollider(enabled) end

---@return boolean
function nodecollider:IsWorldQueryCollider() end


--- =========== Collision filtering ===========

---@param category bitsMask
function nodecollider:SetCollisionCategoryBits(category) end

---@return bitsMask
function nodecollider:GetCollisionCategoryBits() end

---@param mask bitsMask
function nodecollider:SetCollideWithMaskBits(mask) end

---@return bitsMask
function nodecollider:GetCollisionBitsMask() end

