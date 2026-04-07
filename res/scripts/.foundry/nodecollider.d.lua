---@meta

---@class NodeCollider
NodeCollider = {}

---@param name string
function CreateNodeCollider(name) end

--- =========== Local transform (offset from RigidBody) ===========

---@param pos fmath.vec3
function NodeCollider:SetLocalPosition(pos) end

---@param rot fmath.vec3
function NodeCollider:SetLocalRotation(rot) end

function NodeCollider:GetLocalPosition() end
function NodeCollider:GetLocalRotation() end


--- =========== Material ===========

---@param bounciness number [0,1]
function NodeCollider:SetBounciness(bounciness) end

function NodeCollider:GetBounciness() end

---@param friction number
function NodeCollider:SetFrictionCoefficient(friction) end

function NodeCollider:GetFrictionCoefficient() end

---@param density number
function NodeCollider:SetMassDensity(density) end

function NodeCollider:GetMassDensity() end



--- =========== Collider behavior ===========

---@param trigger boolean
function NodeCollider:SetIsTrigger(trigger) end

function NodeCollider:IsTrigger() end

---@param enabled boolean
function NodeCollider:SetIsSimulationCollider(enabled) end
function NodeCollider:IsSimulationCollider() end

---@param enabled boolean
function NodeCollider:SetIsWorldQueryCollider(enabled) end


function NodeCollider:IsWorldQueryCollider() end


--- =========== Collision filtering ===========

---@param category bitsMask
function NodeCollider:SetCollisionCategoryBits(category) end

function NodeCollider:GetCollisionCategoryBits() end

---@param mask bitsMask
function NodeCollider:SetCollideWithMaskBits(mask) end

function NodeCollider:GetCollisionBitsMask() end

