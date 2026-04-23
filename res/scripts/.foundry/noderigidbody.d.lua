---@meta

---@class noderigidbody : node
noderigidbody = {}

---@param name string 
function CreateNodeRigidBody(name) end

---@param force fmaths.vec3
function noderigidbody:ApplyLocalForceAtCenterOfMass(force) end

---@param force fmaths.vec3
---@param point fmaths.vec3
function noderigidbody:ApplyLocalForceAtLocalPosition(force, point) end

---@param force fmaths.vec3
---@param point fmaths.vec3
function noderigidbody:ApplyLocalForceAtWorldPosition(force, point) end


---@param force fmaths.vec3
function noderigidbody:ApplyWorldForceAtCenterOfMass(force) end

---@param force fmaths.vec3
---@param point fmaths.vec3
function noderigidbody:ApplyWorldForceAtLocalPosition(force, point) end

---@param force fmaths.vec3
---@param point fmaths.vec3
function noderigidbody:ApplyWorldForceAtWorldPosition(force, point) end


---@param torque fmaths.vec3
function noderigidbody:ApplyLocalTorque(torque) end

---@param torque fmaths.vec3
function noderigidbody:ApplyWorldTorque(torque) end


---@return fmaths.vec3
function noderigidbody:GetLinearVelocity() end
---@return fmaths.vec3
function noderigidbody:GetAngularVelocity() end
--- Return the linear decelerating factor
---@return number
function noderigidbody:GetLinearDamping() end
---/ Return the angular velocity damping factor
---@return number
function noderigidbody:GetAngularDamping() end
---@return fmaths.vec3
function noderigidbody:GetTotalForce() end


---@param velocity fmaths.vec3
function noderigidbody:SetLinearVelocity(velocity) end

---@param velocity fmaths.vec3
function noderigidbody:SetAngularVelocity(velocity) end

--- set the linear decelerating factor
---@param linearDamping number
function noderigidbody:SetLinearDamping(linearDamping) end

--- set the angular decelerating factor
---@param angularDamping number
function noderigidbody:SetAngularDamping(angularDamping) end


---@param x number
---@param y number
---@param z number
function noderigidbody:LockLinearAxis(x, y, z) end

---@param x number
---@param y number
---@param z number
function noderigidbody:LockAngularAxis(x, y, z) end

function noderigidbody:ResetForces() end
function noderigidbody:ResetTorque() end

--- =========== Mass and Body Type ===========

---@return number
function noderigidbody:GetMass() end

---@param mass number
function noderigidbody:SetMass(mass) end

---@return number
function noderigidbody:GetBodyType() end

---@param type number
function noderigidbody:SetBodyType(type) end

--- =========== Material ===========

--- global ---

---@param bounciness number [0,1]
function noderigidbody:SetBounciness(bounciness) end
---@return number
function noderigidbody:GetBounciness() end

---@param friction number
function noderigidbody:SetFrictionCoefficient(friction) end
---@return number
function noderigidbody:GetFrictionCoefficient() end

---@param density number
function noderigidbody:SetMassDensity(density) end
---@return number
function noderigidbody:GetMassDensity() end

--- specific ---

---@param colliderIndex number
---@param bounciness number
function noderigidbody:SetBounciness(colliderIndex, bounciness) end

---@param colliderIndex number
function noderigidbody:GetBounciness(colliderIndex) end

---@param colliderIndex number
---@param friction number
function noderigidbody:SetFrictionCoefficient(colliderIndex, friction) end

---@param colliderIndex number
function noderigidbody:GetFrictionCoefficient(colliderIndex) end								

---@param colliderIndex number
---@param density number
function noderigidbody:SetMassDensity(colliderIndex, density) end

---@param colliderIndex number
function noderigidbody:GetMassDensity(colliderIndex) end

--- =========== Sleeping and Gravity ===========

---@return boolean
function noderigidbody:IsAllowedToSleep() end
---@return boolean
function noderigidbody:IsSleeping() end
---@return boolean
function noderigidbody:IsGravityEnabled() end

---@param enabled boolean
function noderigidbody:SetSleepingEnabled(enabled) end

---@param isSleeping boolean
function noderigidbody:SetSleepingState(isSleeping) end

---@param enabled boolean
function noderigidbody:SetIsGravityEnabled(enabled) end
